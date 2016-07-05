// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multiphase_mixture_thermo.hpp"
#include "alpha_contact_angle_fv_patch_scalar_field.hpp"
#include "time.hpp"
#include "sub_cycle.hpp"
#include "mules.hpp"
#include "fvc_div.hpp"
#include "fvc_grad.hpp"
#include "fvc_sn_grad.hpp"
#include "fvc_flux.hpp"
#include "fvc_mesh_phi.hpp"
#include "surface_interpolate.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(multiphaseMixtureThermo, 0);

}

const mousse::scalar mousse::multiphaseMixtureThermo::convertToRad =
  mousse::constant::mathematical::pi/180.0;


// Private Member Functions 
void mousse::multiphaseMixtureThermo::calcAlphas()
{
  scalar level = 0.0;
  alphas_ == 0.0;
  FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, phase) {
    alphas_ += level*phase();
    level += 1.0;
  }
  alphas_.correctBoundaryConditions();
}


// Constructors 
mousse::multiphaseMixtureThermo::multiphaseMixtureThermo
(
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  psiThermo{U.mesh(), word::null},
  phases_{lookup("phases"), phaseModel::iNew(p_, T_)},
  mesh_{U.mesh()},
  U_{U},
  phi_{phi},
  rhoPhi_
  {
    {
      "rhoPhi",
      mesh_.time().timeName(),
      mesh_
    },
    mesh_,
    {"rhoPhi", dimMass/dimTime, 0.0}
  },
  alphas_
  {
    {
      "alphas",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"alphas", dimless, 0.0},
    zeroGradientFvPatchScalarField::typeName
  },
  sigmas_{lookup("sigmas")},
  dimSigma_{1, 0, -2, 0, 0},
  deltaN_
  {
    "deltaN",
    1e-8/pow(average(mesh_.V()), 1.0/3.0)
  }
{
  calcAlphas();
  alphas_.write();
  correct();
}


// Member Functions 
void mousse::multiphaseMixtureThermo::correct()
{
  FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, phasei) {
    phasei().correct();
  }
  PtrDictionary<phaseModel>::iterator phasei = phases_.begin();
  psi_ = phasei()*phasei().thermo().psi();
  mu_ = phasei()*phasei().thermo().mu();
  alpha_ = phasei()*phasei().thermo().alpha();
  for (++phasei; phasei != phases_.end(); ++phasei) {
    psi_ += phasei()*phasei().thermo().psi();
    mu_ += phasei()*phasei().thermo().mu();
    alpha_ += phasei()*phasei().thermo().alpha();
  }
}


void mousse::multiphaseMixtureThermo::correctRho(const volScalarField& dp)
{
  FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, phasei) {
    phasei().thermo().rho() +=  phasei().thermo().psi()*dp;
  }
}


bool mousse::multiphaseMixtureThermo::incompressible() const
{
  bool ico = true;
  FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, phases_, phase) {
    ico &= phase().thermo().incompressible();
  }
  return ico;
}


bool mousse::multiphaseMixtureThermo::isochoric() const
{
  bool iso = true;
  FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, phases_, phase) {
    iso &= phase().thermo().incompressible();
  }
  return iso;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::he
(
  const volScalarField& p,
  const volScalarField& T
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> the{phasei()*phasei().thermo().he(p, T)};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    the() += phasei()*phasei().thermo().he(p, T);
  }
  return the;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::he
(
  const scalarField& p,
  const scalarField& T,
  const labelList& cells
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<scalarField> the
  {
    scalarField(phasei(), cells)*phasei().thermo().he(p, T, cells)
  };
  for (++phasei; phasei != phases_.end(); ++phasei) {
    the() += scalarField(phasei(), cells)*phasei().thermo().he(p, T, cells);
  }
  return the;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::he
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<scalarField> the
  {
    phasei().boundaryField()[patchi]*phasei().thermo().he(p, T, patchi)
  };
  for (++phasei; phasei != phases_.end(); ++phasei) {
    the() +=
      phasei().boundaryField()[patchi]*phasei().thermo().he(p, T, patchi);
  }
  return the;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::hc() const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> thc{phasei()*phasei().thermo().hc()};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    thc() += phasei()*phasei().thermo().hc();
  }
  return thc;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::THE
(
  const scalarField& /*h*/,
  const scalarField& /*p*/,
  const scalarField& T0,
  const labelList& /*cells*/
) const
{
  NOT_IMPLEMENTED("multiphaseMixtureThermo::THE(...)");
  return T0;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::THE
(
  const scalarField& /*h*/,
  const scalarField& /*p*/,
  const scalarField& T0,
  const label /*patchi*/
) const
{
  NOT_IMPLEMENTED("multiphaseMixtureThermo::THE(...)");
  return T0;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::rho() const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> trho{phasei()*phasei().thermo().rho()};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    trho() += phasei()*phasei().thermo().rho();
  }
  return trho;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::rho
(
  const label patchi
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<scalarField> trho
  {
    phasei().boundaryField()[patchi]*phasei().thermo().rho(patchi)
  };
  for (++phasei; phasei != phases_.end(); ++phasei) {
    trho() += phasei().boundaryField()[patchi]*phasei().thermo().rho(patchi);
  }
  return trho;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::Cp() const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> tCp{phasei()*phasei().thermo().Cp()};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tCp() += phasei()*phasei().thermo().Cp();
  }
  return tCp;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::Cp
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<scalarField> tCp
  {
    phasei().boundaryField()[patchi]*phasei().thermo().Cp(p, T, patchi)
  };
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tCp() +=
      phasei().boundaryField()[patchi]*phasei().thermo().Cp(p, T, patchi);
  }
  return tCp;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::Cv() const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> tCv{phasei()*phasei().thermo().Cv()};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tCv() += phasei()*phasei().thermo().Cv();
  }
  return tCv;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::Cv
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<scalarField> tCv
  {
    phasei().boundaryField()[patchi]*phasei().thermo().Cv(p, T, patchi)
  };
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tCv() += phasei().boundaryField()[patchi]*phasei().thermo().Cv(p, T, patchi);
  }
  return tCv;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::gamma() const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> tgamma{phasei()*phasei().thermo().gamma()};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tgamma() += phasei()*phasei().thermo().gamma();
  }
  return tgamma;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::gamma
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<scalarField> tgamma
  {
    phasei().boundaryField()[patchi]*phasei().thermo().gamma(p, T, patchi)
  };
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tgamma() +=
      phasei().boundaryField()[patchi]*phasei().thermo().gamma(p, T, patchi);
  }
  return tgamma;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::Cpv() const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> tCpv{phasei()*phasei().thermo().Cpv()};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tCpv() += phasei()*phasei().thermo().Cpv();
  }
  return tCpv;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::Cpv
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<scalarField> tCpv
  {
    phasei().boundaryField()[patchi]*phasei().thermo().Cpv(p, T, patchi)
  };
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tCpv() +=
      phasei().boundaryField()[patchi]*phasei().thermo().Cpv(p, T, patchi);
  }
  return tCpv;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::CpByCpv() const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> tCpByCpv{phasei()*phasei().thermo().CpByCpv()};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tCpByCpv() += phasei()*phasei().thermo().CpByCpv();
  }
  return tCpByCpv;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::CpByCpv
(
  const scalarField& p,
  const scalarField& T,
  const label patchi
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<scalarField> tCpByCpv
  {
    phasei().boundaryField()[patchi]*phasei().thermo().CpByCpv(p, T, patchi)
  };
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tCpByCpv() +=
      phasei().boundaryField()[patchi]*phasei().thermo().CpByCpv(p, T, patchi);
  }
  return tCpByCpv;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::nu() const
{
  return mu()/rho();
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::nu
(
  const label patchi
) const
{
  return mu(patchi)/rho(patchi);
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::kappa() const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> tkappa{phasei()*phasei().thermo().kappa()};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tkappa() += phasei()*phasei().thermo().kappa();
  }
  return tkappa;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::kappa
(
  const label patchi
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<scalarField> tkappa
  {
    phasei().boundaryField()[patchi]*phasei().thermo().kappa(patchi)
  };
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tkappa() +=
      phasei().boundaryField()[patchi]*phasei().thermo().kappa(patchi);
  }
  return tkappa;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::kappaEff
(
  const volScalarField& alphat
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> tkappaEff{phasei()*phasei().thermo().kappaEff(alphat)};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tkappaEff() += phasei()*phasei().thermo().kappaEff(alphat);
  }
  return tkappaEff;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::kappaEff
(
  const scalarField& alphat,
  const label patchi
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<scalarField> tkappaEff
  {
    phasei().boundaryField()[patchi]*phasei().thermo().kappaEff(alphat, patchi)
  };
  for (++phasei; phasei != phases_.end(); ++phasei) {
    tkappaEff() +=
      phasei().boundaryField()[patchi]*phasei().thermo().kappaEff(alphat, patchi);
  }
  return tkappaEff;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::alphaEff
(
  const volScalarField& alphat
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> talphaEff{phasei()*phasei().thermo().alphaEff(alphat)};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    talphaEff() += phasei()*phasei().thermo().alphaEff(alphat);
  }
  return talphaEff;
}


mousse::tmp<mousse::scalarField> mousse::multiphaseMixtureThermo::alphaEff
(
  const scalarField& alphat,
  const label patchi
) const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<scalarField> talphaEff
  {
    phasei().boundaryField()[patchi]*phasei().thermo().alphaEff(alphat, patchi)
  };
  for (++phasei; phasei != phases_.end(); ++phasei) {
    talphaEff() +=
      phasei().boundaryField()[patchi]*phasei().thermo().alphaEff(alphat, patchi);
  }
  return talphaEff;
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::rCv() const
{
  PtrDictionary<phaseModel>::const_iterator phasei = phases_.begin();
  tmp<volScalarField> trCv{phasei()/phasei().thermo().Cv()};
  for (++phasei; phasei != phases_.end(); ++phasei) {
    trCv() += phasei()/phasei().thermo().Cv();
  }
  return trCv;
}


mousse::tmp<mousse::surfaceScalarField>
mousse::multiphaseMixtureThermo::surfaceTensionForce() const
{
  tmp<surfaceScalarField> tstf
  {
    new surfaceScalarField
    {
      {
        "surfaceTensionForce",
        mesh_.time().timeName(),
        mesh_
      },
      mesh_,
      {
        "surfaceTensionForce",
        dimensionSet(1, -2, -2, 0, 0),
        0.0
      }
    }
  };
  surfaceScalarField& stf = tstf();
  FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, phases_, phase1) {
    const phaseModel& alpha1 = phase1();
    PtrDictionary<phaseModel>::const_iterator phase2 = phase1;
    ++phase2;
    for (; phase2 != phases_.end(); ++phase2) {
      const phaseModel& alpha2 = phase2();
      sigmaTable::const_iterator sigma =
        sigmas_.find(interfacePair(alpha1, alpha2));
      if (sigma == sigmas_.end()) {
        FATAL_ERROR_IN
        (
          "multiphaseMixtureThermo::surfaceTensionForce() const"
        )
        << "Cannot find interface " << interfacePair(alpha1, alpha2)
        << " in list of sigma values"
        << exit(FatalError);
      }
      stf += dimensionedScalar("sigma", dimSigma_, sigma())
       *fvc::interpolate(K(alpha1, alpha2))*
        (
          fvc::interpolate(alpha2)*fvc::snGrad(alpha1)
          - fvc::interpolate(alpha1)*fvc::snGrad(alpha2)
        );
    }
  }
  return tstf;
}


void mousse::multiphaseMixtureThermo::solve()
{
  const Time& runTime = mesh_.time();
  const dictionary& alphaControls = mesh_.solverDict("alpha");
  label nAlphaSubCycles{readLabel(alphaControls.lookup("nAlphaSubCycles"))};
  scalar cAlpha{readScalar(alphaControls.lookup("cAlpha"))};
  volScalarField& alpha = phases_.first();
  if (nAlphaSubCycles > 1) {
    surfaceScalarField rhoPhiSum{0.0*rhoPhi_};
    dimensionedScalar totalDeltaT = runTime.deltaT();
    for (subCycle<volScalarField> alphaSubCycle(alpha, nAlphaSubCycles);
         !(++alphaSubCycle).end();) {
      solveAlphas(cAlpha);
      rhoPhiSum += (runTime.deltaT()/totalDeltaT)*rhoPhi_;
    }
    rhoPhi_ = rhoPhiSum;
  } else {
    solveAlphas(cAlpha);
  }
}


mousse::tmp<mousse::surfaceVectorField> mousse::multiphaseMixtureThermo::nHatfv
(
  const volScalarField& alpha1,
  const volScalarField& alpha2
) const
{
  surfaceVectorField gradAlphaf
  {
    fvc::interpolate(alpha2)*fvc::interpolate(fvc::grad(alpha1))
    - fvc::interpolate(alpha1)*fvc::interpolate(fvc::grad(alpha2))
  };
  // Face unit interface normal
  return gradAlphaf/(mag(gradAlphaf) + deltaN_);
}


mousse::tmp<mousse::surfaceScalarField> mousse::multiphaseMixtureThermo::nHatf
(
  const volScalarField& alpha1,
  const volScalarField& alpha2
) const
{
  // Face unit interface normal flux
  return nHatfv(alpha1, alpha2) & mesh_.Sf();
}


// Correction for the boundary condition on the unit normal nHat on
// walls to produce the correct contact angle.
// The dynamic contact angle is calculated from the component of the
// velocity on the direction of the interface, parallel to the wall.
void mousse::multiphaseMixtureThermo::correctContactAngle
(
  const phaseModel& alpha1,
  const phaseModel& alpha2,
  surfaceVectorField::GeometricBoundaryField& nHatb
) const
{
  const volScalarField::GeometricBoundaryField& gbf
    = alpha1.boundaryField();
  const fvBoundaryMesh& boundary = mesh_.boundary();
  FOR_ALL(boundary, patchi) {
    if (!isA<alphaContactAngleFvPatchScalarField>(gbf[patchi]))
      continue;
    const alphaContactAngleFvPatchScalarField& acap =
      refCast<const alphaContactAngleFvPatchScalarField>(gbf[patchi]);
    vectorField& nHatPatch = nHatb[patchi];
    vectorField AfHatPatch
    {
      mesh_.Sf().boundaryField()[patchi]/mesh_.magSf().boundaryField()[patchi]
    };
    alphaContactAngleFvPatchScalarField::thetaPropsTable::
      const_iterator tp = acap.thetaProps().find(interfacePair(alpha1, alpha2));
    if (tp == acap.thetaProps().end()) {
      FATAL_ERROR_IN
      (
        "multiphaseMixtureThermo::correctContactAngle"
        "(const phaseModel& alpha1, const phaseModel& alpha2, "
        "fvPatchVectorFieldField& nHatb) const"
      )
      << "Cannot find interface " << interfacePair(alpha1, alpha2)
      << "\n    in table of theta properties for patch "
      << acap.patch().name()
      << exit(FatalError);
    }
    bool matched = (tp.key().first() == alpha1.name());
    scalar theta0 = convertToRad*tp().theta0(matched);
    scalarField theta{boundary[patchi].size(), theta0};
    scalar uTheta = tp().uTheta();
    // Calculate the dynamic contact angle if required
    if (uTheta > SMALL) {
      scalar thetaA = convertToRad*tp().thetaA(matched);
      scalar thetaR = convertToRad*tp().thetaR(matched);
      // Calculated the component of the velocity parallel to the wall
      vectorField Uwall
      {
        U_.boundaryField()[patchi].patchInternalField()
        - U_.boundaryField()[patchi]
      };
      Uwall -= (AfHatPatch & Uwall)*AfHatPatch;
      // Find the direction of the interface parallel to the wall
      vectorField nWall
      {
        nHatPatch - (AfHatPatch & nHatPatch)*AfHatPatch
      };
      // Normalise nWall
      nWall /= (mag(nWall) + SMALL);
      // Calculate Uwall resolved normal to the interface parallel to
      // the interface
      scalarField uwall{nWall & Uwall};
      theta += (thetaA - thetaR)*tanh(uwall/uTheta);
    }
    // Reset nHatPatch to correspond to the contact angle
    scalarField a12{nHatPatch & AfHatPatch};
    scalarField b1{cos(theta)};
    scalarField b2{nHatPatch.size()};
    FOR_ALL(b2, facei) {
      b2[facei] = cos(acos(a12[facei]) - theta[facei]);
    }
    scalarField det{1.0 - a12*a12};
    scalarField a{(b1 - a12*b2)/det};
    scalarField b{(b2 - a12*b1)/det};
    nHatPatch = a*AfHatPatch + b*nHatPatch;
    nHatPatch /= (mag(nHatPatch) + deltaN_.value());
  }
}


mousse::tmp<mousse::volScalarField> mousse::multiphaseMixtureThermo::K
(
  const phaseModel& alpha1,
  const phaseModel& alpha2
) const
{
  tmp<surfaceVectorField> tnHatfv = nHatfv(alpha1, alpha2);
  correctContactAngle(alpha1, alpha2, tnHatfv().boundaryField());
  // Simple expression for curvature
  return -fvc::div(tnHatfv & mesh_.Sf());
}


mousse::tmp<mousse::volScalarField>
mousse::multiphaseMixtureThermo::nearInterface() const
{
  tmp<volScalarField> tnearInt
  {
    new volScalarField
    {
      {
        "nearInterface",
        mesh_.time().timeName(),
        mesh_
      },
      mesh_,
      {"nearInterface", dimless, 0.0}
    }
  };
  FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, phases_, phase) {
    tnearInt() = max(tnearInt(), pos(phase() - 0.01)*pos(0.99 - phase()));
  }
  return tnearInt;
}


void mousse::multiphaseMixtureThermo::solveAlphas
(
  const scalar cAlpha
)
{
  static label nSolves=-1;
  nSolves++;
  word alphaScheme{"div(phi,alpha)"};
  word alpharScheme{"div(phirb,alpha)"};
  surfaceScalarField phic{mag(phi_/mesh_.magSf())};
  phic = min(cAlpha*phic, max(phic));
  PtrList<surfaceScalarField> alphaPhiCorrs{phases_.size()};
  int phasei = 0;
  FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, phase) {
    phaseModel& alpha = phase();
    alphaPhiCorrs.set
    (
      phasei,
      new surfaceScalarField
      {
        phi_.name() + alpha.name(),
        fvc::flux
        (
          phi_,
          alpha,
          alphaScheme
        )
      }
    );
    surfaceScalarField& alphaPhiCorr = alphaPhiCorrs[phasei];
    FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, phase2) {
      phaseModel& alpha2 = phase2();
      if (&alpha2 == &alpha)
        continue;
      surfaceScalarField phir{phic*nHatf(alpha, alpha2)};
      alphaPhiCorr += fvc::flux
      (
        -fvc::flux(-phir, alpha2, alpharScheme),
        alpha,
        alpharScheme
      );
    }
    MULES::limit
    (
      1.0/mesh_.time().deltaT().value(),
      geometricOneField(),
      alpha,
      phi_,
      alphaPhiCorr,
      zeroField(),
      zeroField(),
      1,
      0,
      true
    );
    phasei++;
  }
  MULES::limitSum(alphaPhiCorrs);
  rhoPhi_ = {"0", {1, 0, -1, 0, 0}, 0};
  volScalarField sumAlpha
  {
    {
      "sumAlpha",
      mesh_.time().timeName(),
      mesh_
    },
    mesh_,
    {"sumAlpha", dimless, 0}
  };
  volScalarField divU{fvc::div(fvc::absolute(phi_, U_))};
  phasei = 0;
  FOR_ALL_ITER(PtrDictionary<phaseModel>, phases_, phase) {
    phaseModel& alpha = phase();
    surfaceScalarField& alphaPhi = alphaPhiCorrs[phasei];
    alphaPhi += upwind<scalar>(mesh_, phi_).flux(alpha);
    volScalarField::DimensionedInternalField Sp
    {
      {
        "Sp",
        mesh_.time().timeName(),
        mesh_
      },
      mesh_,
      {"Sp", alpha.dgdt().dimensions(), 0.0}
    };
    volScalarField::DimensionedInternalField Su
    {
      {
        "Su",
        mesh_.time().timeName(),
        mesh_
      },
      // Divergence term is handled explicitly to be
      // consistent with the explicit transport solution
      divU*min(alpha, scalar(1))
    };

    {
      const scalarField& dgdt = alpha.dgdt();
      FOR_ALL(dgdt, celli) {
        if (dgdt[celli] < 0.0 && alpha[celli] > 0.0) {
          Sp[celli] += dgdt[celli]*alpha[celli];
          Su[celli] -= dgdt[celli]*alpha[celli];
        } else if (dgdt[celli] > 0.0 && alpha[celli] < 1.0) {
          Sp[celli] -= dgdt[celli]*(1.0 - alpha[celli]);
        }
      }
    }

    FOR_ALL_CONST_ITER(PtrDictionary<phaseModel>, phases_, phase2) {
      const phaseModel& alpha2 = phase2();
      if (&alpha2 == &alpha)
        continue;
      const scalarField& dgdt2 = alpha2.dgdt();
      FOR_ALL(dgdt2, celli) {
        if (dgdt2[celli] > 0.0 && alpha2[celli] < 1.0) {
          Sp[celli] -= dgdt2[celli]*(1.0 - alpha2[celli]);
          Su[celli] += dgdt2[celli]*alpha[celli];
        } else if (dgdt2[celli] < 0.0 && alpha2[celli] > 0.0) {
          Sp[celli] += dgdt2[celli]*alpha2[celli];
        }
      }
    }
    MULES::explicitSolve
    (
      geometricOneField(),
      alpha,
      alphaPhi,
      Sp,
      Su
    );
    rhoPhi_ += fvc::interpolate(alpha.thermo().rho())*alphaPhi;
    Info << alpha.name() << " volume fraction, min, max = "
      << alpha.weightedAverage(mesh_.V()).value()
      << ' ' << min(alpha).value()
      << ' ' << max(alpha).value()
      << endl;
    sumAlpha += alpha;
    phasei++;
  }
  Info << "Phase-sum volume fraction, min, max = "
    << sumAlpha.weightedAverage(mesh_.V()).value()
    << ' ' << min(sumAlpha).value()
    << ' ' << max(sumAlpha).value()
    << endl;
  calcAlphas();
}

