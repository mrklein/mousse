// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mules.hpp"
#include "upwind.hpp"
#include "fvc_surface_integrate.hpp"
#include "local_euler_ddt_scheme.hpp"
#include "sliced_surface_fields.hpp"
#include "wedge_fv_patch.hpp"
#include "sync_tools.hpp"
template<class RdeltaTType, class RhoType, class SpType, class SuType>
void mousse::MULES::explicitSolve
(
  const RdeltaTType& rDeltaT,
  const RhoType& rho,
  volScalarField& psi,
  const surfaceScalarField& phiPsi,
  const SpType& Sp,
  const SuType& Su
)
{
  Info<< "MULES: Solving for " << psi.name() << endl;
  const fvMesh& mesh = psi.mesh();
  scalarField& psiIf = psi;
  const scalarField& psi0 = psi.oldTime();
  psiIf = 0.0;
  fvc::surfaceIntegrate(psiIf, phiPsi);
  if (mesh.moving())
  {
    psiIf =
    (
      mesh.Vsc0()().field()*rho.oldTime().field()
     *psi0*rDeltaT/mesh.Vsc()().field()
     + Su.field()
     - psiIf
    )/(rho.field()*rDeltaT - Sp.field());
  }
  else
  {
    psiIf =
    (
      rho.oldTime().field()*psi0*rDeltaT
     + Su.field()
     - psiIf
    )/(rho.field()*rDeltaT - Sp.field());
  }
  psi.correctBoundaryConditions();
}
template<class RhoType, class SpType, class SuType>
void mousse::MULES::explicitSolve
(
  const RhoType& rho,
  volScalarField& psi,
  const surfaceScalarField& phiPsi,
  const SpType& Sp,
  const SuType& Su
)
{
  const fvMesh& mesh = psi.mesh();
  if (fv::localEulerDdt::enabled(mesh))
  {
    const volScalarField& rDeltaT = fv::localEulerDdt::localRDeltaT(mesh);
    explicitSolve(rDeltaT, rho, psi, phiPsi, Sp, Su);
  }
  else
  {
    const scalar rDeltaT = 1.0/mesh.time().deltaTValue();
    explicitSolve(rDeltaT, rho, psi, phiPsi, Sp, Su);
  }
}
template<class RhoType, class SpType, class SuType>
void mousse::MULES::explicitSolve
(
  const RhoType& rho,
  volScalarField& psi,
  const surfaceScalarField& phi,
  surfaceScalarField& phiPsi,
  const SpType& Sp,
  const SuType& Su,
  const scalar psiMax,
  const scalar psiMin
)
{
  const fvMesh& mesh = psi.mesh();
  psi.correctBoundaryConditions();
  if (fv::localEulerDdt::enabled(mesh))
  {
    const volScalarField& rDeltaT = fv::localEulerDdt::localRDeltaT(mesh);
    limit
    (
      rDeltaT,
      rho,
      psi,
      phi,
      phiPsi,
      Sp,
      Su,
      psiMax,
      psiMin,
      false
    );
    explicitSolve(rDeltaT, rho, psi, phiPsi, Sp, Su);
  }
  else
  {
    const scalar rDeltaT = 1.0/mesh.time().deltaTValue();
    limit
    (
      rDeltaT,
      rho,
      psi,
      phi,
      phiPsi,
      Sp,
      Su,
      psiMax,
      psiMin,
      false
    );
    explicitSolve(rDeltaT, rho, psi, phiPsi, Sp, Su);
  }
}
template<class RdeltaTType, class RhoType, class SpType, class SuType>
void mousse::MULES::limiter
(
  scalarField& allLambda,
  const RdeltaTType& rDeltaT,
  const RhoType& rho,
  const volScalarField& psi,
  const surfaceScalarField& phiBD,
  const surfaceScalarField& phiCorr,
  const SpType& Sp,
  const SuType& Su,
  const scalar psiMax,
  const scalar psiMin
)
{
  const scalarField& psiIf = psi;
  const volScalarField::GeometricBoundaryField& psiBf = psi.boundaryField();
  const fvMesh& mesh = psi.mesh();
  const dictionary& MULEScontrols = mesh.solverDict(psi.name());
  label nLimiterIter
  (
    MULEScontrols.lookupOrDefault<label>("nLimiterIter", 3)
  );
  scalar smoothLimiter
  (
    MULEScontrols.lookupOrDefault<scalar>("smoothLimiter", 0)
  );
  const scalarField& psi0 = psi.oldTime();
  const labelUList& owner = mesh.owner();
  const labelUList& neighb = mesh.neighbour();
  tmp<volScalarField::DimensionedInternalField> tVsc = mesh.Vsc();
  const scalarField& V = tVsc();
  const scalarField& phiBDIf = phiBD;
  const surfaceScalarField::GeometricBoundaryField& phiBDBf =
    phiBD.boundaryField();
  const scalarField& phiCorrIf = phiCorr;
  const surfaceScalarField::GeometricBoundaryField& phiCorrBf =
    phiCorr.boundaryField();
  slicedSurfaceScalarField lambda
  (
    IOobject
    (
      "lambda",
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    ),
    mesh,
    dimless,
    allLambda,
    false   // Use slices for the couples
  );
  scalarField& lambdaIf = lambda;
  surfaceScalarField::GeometricBoundaryField& lambdaBf =
    lambda.boundaryField();
  scalarField psiMaxn(psiIf.size(), psiMin);
  scalarField psiMinn(psiIf.size(), psiMax);
  scalarField sumPhiBD(psiIf.size(), 0.0);
  scalarField sumPhip(psiIf.size(), VSMALL);
  scalarField mSumPhim(psiIf.size(), VSMALL);
  FOR_ALL(phiCorrIf, facei)
  {
    label own = owner[facei];
    label nei = neighb[facei];
    psiMaxn[own] = max(psiMaxn[own], psiIf[nei]);
    psiMinn[own] = min(psiMinn[own], psiIf[nei]);
    psiMaxn[nei] = max(psiMaxn[nei], psiIf[own]);
    psiMinn[nei] = min(psiMinn[nei], psiIf[own]);
    sumPhiBD[own] += phiBDIf[facei];
    sumPhiBD[nei] -= phiBDIf[facei];
    scalar phiCorrf = phiCorrIf[facei];
    if (phiCorrf > 0.0)
    {
      sumPhip[own] += phiCorrf;
      mSumPhim[nei] += phiCorrf;
    }
    else
    {
      mSumPhim[own] -= phiCorrf;
      sumPhip[nei] -= phiCorrf;
    }
  }
  FOR_ALL(phiCorrBf, patchi)
  {
    const fvPatchScalarField& psiPf = psiBf[patchi];
    const scalarField& phiBDPf = phiBDBf[patchi];
    const scalarField& phiCorrPf = phiCorrBf[patchi];
    const labelList& pFaceCells = mesh.boundary()[patchi].faceCells();
    if (psiPf.coupled())
    {
      const scalarField psiPNf(psiPf.patchNeighbourField());
      FOR_ALL(phiCorrPf, pFacei)
      {
        label pfCelli = pFaceCells[pFacei];
        psiMaxn[pfCelli] = max(psiMaxn[pfCelli], psiPNf[pFacei]);
        psiMinn[pfCelli] = min(psiMinn[pfCelli], psiPNf[pFacei]);
      }
    }
    else
    {
      FOR_ALL(phiCorrPf, pFacei)
      {
        label pfCelli = pFaceCells[pFacei];
        psiMaxn[pfCelli] = max(psiMaxn[pfCelli], psiPf[pFacei]);
        psiMinn[pfCelli] = min(psiMinn[pfCelli], psiPf[pFacei]);
      }
    }
    FOR_ALL(phiCorrPf, pFacei)
    {
      label pfCelli = pFaceCells[pFacei];
      sumPhiBD[pfCelli] += phiBDPf[pFacei];
      scalar phiCorrf = phiCorrPf[pFacei];
      if (phiCorrf > 0.0)
      {
        sumPhip[pfCelli] += phiCorrf;
      }
      else
      {
        mSumPhim[pfCelli] -= phiCorrf;
      }
    }
  }
  psiMaxn = min(psiMaxn, psiMax);
  psiMinn = max(psiMinn, psiMin);
  if (smoothLimiter > SMALL)
  {
    psiMaxn =
      min(smoothLimiter*psiIf + (1.0 - smoothLimiter)*psiMaxn, psiMax);
    psiMinn =
      max(smoothLimiter*psiIf + (1.0 - smoothLimiter)*psiMinn, psiMin);
  }
  if (mesh.moving())
  {
    tmp<volScalarField::DimensionedInternalField> V0 = mesh.Vsc0();
    psiMaxn =
      V
     *(
       (rho.field()*rDeltaT - Sp.field())*psiMaxn
      - Su.field()
      )
     - (V0().field()*rDeltaT)*rho.oldTime().field()*psi0
     + sumPhiBD;
    psiMinn =
      V
     *(
       Su.field()
      - (rho.field()*rDeltaT - Sp.field())*psiMinn
      )
     + (V0().field()*rDeltaT)*rho.oldTime().field()*psi0
     - sumPhiBD;
  }
  else
  {
    psiMaxn =
      V
     *(
       (rho.field()*rDeltaT - Sp.field())*psiMaxn
      - Su.field()
      - (rho.oldTime().field()*rDeltaT)*psi0
      )
     + sumPhiBD;
    psiMinn =
      V
     *(
       Su.field()
      - (rho.field()*rDeltaT - Sp.field())*psiMinn
      + (rho.oldTime().field()*rDeltaT)*psi0
      )
     - sumPhiBD;
  }
  scalarField sumlPhip(psiIf.size());
  scalarField mSumlPhim(psiIf.size());
  for (int j=0; j<nLimiterIter; j++)
  {
    sumlPhip = 0.0;
    mSumlPhim = 0.0;
    FOR_ALL(lambdaIf, facei)
    {
      label own = owner[facei];
      label nei = neighb[facei];
      scalar lambdaPhiCorrf = lambdaIf[facei]*phiCorrIf[facei];
      if (lambdaPhiCorrf > 0.0)
      {
        sumlPhip[own] += lambdaPhiCorrf;
        mSumlPhim[nei] += lambdaPhiCorrf;
      }
      else
      {
        mSumlPhim[own] -= lambdaPhiCorrf;
        sumlPhip[nei] -= lambdaPhiCorrf;
      }
    }
    FOR_ALL(lambdaBf, patchi)
    {
      scalarField& lambdaPf = lambdaBf[patchi];
      const scalarField& phiCorrfPf = phiCorrBf[patchi];
      const labelList& pFaceCells = mesh.boundary()[patchi].faceCells();
      FOR_ALL(lambdaPf, pFacei)
      {
        label pfCelli = pFaceCells[pFacei];
        scalar lambdaPhiCorrf = lambdaPf[pFacei]*phiCorrfPf[pFacei];
        if (lambdaPhiCorrf > 0.0)
        {
          sumlPhip[pfCelli] += lambdaPhiCorrf;
        }
        else
        {
          mSumlPhim[pfCelli] -= lambdaPhiCorrf;
        }
      }
    }
    FOR_ALL(sumlPhip, celli)
    {
      sumlPhip[celli] =
        max(min
        (
          (sumlPhip[celli] + psiMaxn[celli])
         /(mSumPhim[celli] - SMALL),
          1.0), 0.0
        );
      mSumlPhim[celli] =
        max(min
        (
          (mSumlPhim[celli] + psiMinn[celli])
         /(sumPhip[celli] + SMALL),
          1.0), 0.0
        );
    }
    const scalarField& lambdam = sumlPhip;
    const scalarField& lambdap = mSumlPhim;
    FOR_ALL(lambdaIf, facei)
    {
      if (phiCorrIf[facei] > 0.0)
      {
        lambdaIf[facei] = min
        (
          lambdaIf[facei],
          min(lambdap[owner[facei]], lambdam[neighb[facei]])
        );
      }
      else
      {
        lambdaIf[facei] = min
        (
          lambdaIf[facei],
          min(lambdam[owner[facei]], lambdap[neighb[facei]])
        );
      }
    }
    FOR_ALL(lambdaBf, patchi)
    {
      fvsPatchScalarField& lambdaPf = lambdaBf[patchi];
      const scalarField& phiCorrfPf = phiCorrBf[patchi];
      const fvPatchScalarField& psiPf = psiBf[patchi];
      if (isA<wedgeFvPatch>(mesh.boundary()[patchi]))
      {
        lambdaPf = 0;
      }
      else if (psiPf.coupled())
      {
        const labelList& pFaceCells =
          mesh.boundary()[patchi].faceCells();
        FOR_ALL(lambdaPf, pFacei)
        {
          label pfCelli = pFaceCells[pFacei];
          if (phiCorrfPf[pFacei] > 0.0)
          {
            lambdaPf[pFacei] =
              min(lambdaPf[pFacei], lambdap[pfCelli]);
          }
          else
          {
            lambdaPf[pFacei] =
              min(lambdaPf[pFacei], lambdam[pfCelli]);
          }
        }
      }
      else
      {
        const labelList& pFaceCells =
          mesh.boundary()[patchi].faceCells();
        const scalarField& phiBDPf = phiBDBf[patchi];
        const scalarField& phiCorrPf = phiCorrBf[patchi];
        FOR_ALL(lambdaPf, pFacei)
        {
          // Limit outlet faces only
          if ((phiBDPf[pFacei] + phiCorrPf[pFacei]) > SMALL*SMALL)
          {
            label pfCelli = pFaceCells[pFacei];
            if (phiCorrfPf[pFacei] > 0.0)
            {
              lambdaPf[pFacei] =
                min(lambdaPf[pFacei], lambdap[pfCelli]);
            }
            else
            {
              lambdaPf[pFacei] =
                min(lambdaPf[pFacei], lambdam[pfCelli]);
            }
          }
        }
      }
    }
    syncTools::syncFaceList(mesh, allLambda, minEqOp<scalar>());
  }
}
template<class RdeltaTType, class RhoType, class SpType, class SuType>
void mousse::MULES::limit
(
  const RdeltaTType& rDeltaT,
  const RhoType& rho,
  const volScalarField& psi,
  const surfaceScalarField& phi,
  surfaceScalarField& phiPsi,
  const SpType& Sp,
  const SuType& Su,
  const scalar psiMax,
  const scalar psiMin,
  const bool returnCorr
)
{
  const fvMesh& mesh = psi.mesh();
  surfaceScalarField phiBD(upwind<scalar>(psi.mesh(), phi).flux(psi));
  surfaceScalarField& phiCorr = phiPsi;
  phiCorr -= phiBD;
  scalarField allLambda(mesh.nFaces(), 1.0);
  slicedSurfaceScalarField lambda
  (
    IOobject
    (
      "lambda",
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    ),
    mesh,
    dimless,
    allLambda,
    false   // Use slices for the couples
  );
  limiter
  (
    allLambda,
    rDeltaT,
    rho,
    psi,
    phiBD,
    phiCorr,
    Sp,
    Su,
    psiMax,
    psiMin
  );
  if (returnCorr)
  {
    phiCorr *= lambda;
  }
  else
  {
    phiPsi = phiBD + lambda*phiCorr;
  }
}
template<class SurfaceScalarFieldList>
void mousse::MULES::limitSum(SurfaceScalarFieldList& phiPsiCorrs)
{
  {
    UPtrList<scalarField> phiPsiCorrsInternal(phiPsiCorrs.size());
    FOR_ALL(phiPsiCorrs, phasei)
    {
      phiPsiCorrsInternal.set(phasei, &phiPsiCorrs[phasei]);
    }
    limitSum(phiPsiCorrsInternal);
  }
  surfaceScalarField::GeometricBoundaryField& bfld =
    phiPsiCorrs[0].boundaryField();
  FOR_ALL(bfld, patchi)
  {
    if (bfld[patchi].coupled())
    {
      UPtrList<scalarField> phiPsiCorrsPatch(phiPsiCorrs.size());
      FOR_ALL(phiPsiCorrs, phasei)
      {
        phiPsiCorrsPatch.set
        (
          phasei,
          &phiPsiCorrs[phasei].boundaryField()[patchi]
        );
      }
      limitSum(phiPsiCorrsPatch);
    }
  }
}
