// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "imules.hpp"
#include "gauss_convection_scheme.hpp"
#include "surface_interpolate.hpp"
#include "fvm_ddt.hpp"
#include "fvm_sup.hpp"
#include "fvc_div.hpp"
namespace mousse
{
namespace MULES
{
  template<class RhoType>
  inline tmp<surfaceScalarField> interpolate(const RhoType& rho)
  {
    notImplemented
    (
      "tmp<surfaceScalarField> interpolate(const RhoType& rho)"
    );
    return tmp<surfaceScalarField>(NULL);
  }
  template<>
  inline tmp<surfaceScalarField> interpolate(const volScalarField& rho)
  {
    return fvc::interpolate(rho);
  }
}
}
template<class RhoType, class SpType, class SuType>
void mousse::MULES::implicitSolve
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
  const dictionary& MULEScontrols = mesh.solverDict(psi.name());
  label maxIter
  (
    readLabel(MULEScontrols.lookup("maxIter"))
  );
  scalar maxUnboundedness
  (
    readScalar(MULEScontrols.lookup("maxUnboundedness"))
  );
  scalar CoCoeff
  (
    readScalar(MULEScontrols.lookup("CoCoeff"))
  );
  scalarField allCoLambda(mesh.nFaces());
  {
    slicedSurfaceScalarField CoLambda
    (
      IOobject
      (
        "CoLambda",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      mesh,
      dimless,
      allCoLambda,
      false   // Use slices for the couples
    );
    if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
    {
      tmp<surfaceScalarField> Cof =
        mesh.time().deltaT()*mesh.surfaceInterpolation::deltaCoeffs()
       *mag(phi/interpolate(rho))/mesh.magSf();
      CoLambda == 1.0/max(CoCoeff*Cof, scalar(1));
    }
    else
    {
      tmp<surfaceScalarField> Cof =
        mesh.time().deltaT()*mesh.surfaceInterpolation::deltaCoeffs()
       *mag(phi)/mesh.magSf();
      CoLambda == 1.0/max(CoCoeff*Cof, scalar(1));
    }
  }
  scalarField allLambda(allCoLambda);
  //scalarField allLambda(mesh.nFaces(), 1.0);
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
  linear<scalar> CDs(mesh);
  upwind<scalar> UDs(mesh, phi);
  //fv::uncorrectedSnGrad<scalar> snGrads(mesh);
  fvScalarMatrix psiConvectionDiffusion
  (
    fvm::ddt(rho, psi)
   + fv::gaussConvectionScheme<scalar>(mesh, phi, UDs).fvmDiv(phi, psi)
    //- fv::gaussLaplacianScheme<scalar, scalar>(mesh, CDs, snGrads)
    //.fvmLaplacian(Dpsif, psi)
   - fvm::Sp(Sp, psi)
   - Su
  );
  surfaceScalarField phiBD(psiConvectionDiffusion.flux());
  surfaceScalarField& phiCorr = phiPsi;
  phiCorr -= phiBD;
  for (label i=0; i<maxIter; i++)
  {
    if (i != 0 && i < 4)
    {
      allLambda = allCoLambda;
    }
    limiter
    (
      allLambda,
      1.0/mesh.time().deltaTValue(),
      rho,
      psi,
      phiBD,
      phiCorr,
      Sp,
      Su,
      psiMax,
      psiMin
    );
    solve
    (
      psiConvectionDiffusion + fvc::div(lambda*phiCorr),
      MULEScontrols
    );
    scalar maxPsiM1 = gMax(psi.internalField()) - 1.0;
    scalar minPsi = gMin(psi.internalField());
    scalar unboundedness = max(max(maxPsiM1, 0.0), -min(minPsi, 0.0));
    if (unboundedness < maxUnboundedness)
    {
      break;
    }
    else
    {
      Info<< "MULES: max(" << psi.name() << " - 1) = " << maxPsiM1
        << " min(" << psi.name() << ") = " << minPsi << endl;
      phiBD = psiConvectionDiffusion.flux();
      /*
      word gammaScheme("div(phi,gamma)");
      word gammarScheme("div(phirb,gamma)");
      const surfaceScalarField& phir =
        mesh.lookupObject<surfaceScalarField>("phir");
      phiCorr =
        fvc::flux
        (
          phi,
          psi,
          gammaScheme
        )
       + fvc::flux
        (
          -fvc::flux(-phir, scalar(1) - psi, gammarScheme),
          psi,
          gammarScheme
        )
        - phiBD;
      */
    }
  }
  phiPsi = psiConvectionDiffusion.flux() + lambda*phiCorr;
}
