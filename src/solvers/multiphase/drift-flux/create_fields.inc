Info << "Reading field p_rgh\n" << endl;
volScalarField p_rgh
{
  {
    "p_rgh",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
Info << "Reading field U\n" << endl;
volVectorField U
{
  {
    "U",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
#include "create_phi.inc"
Info << "Reading incompressibleTwoPhaseInteractingMixture\n" << endl;
incompressibleTwoPhaseInteractingMixture mixture{U, phi};
volScalarField& alpha1 = mixture.alpha1();
volScalarField& alpha2 = mixture.alpha2();
const dimensionedScalar& rho1 = mixture.rhod();
const dimensionedScalar& rho2 = mixture.rhoc();
// Mixture density
volScalarField rho
{
  {
    "rho",
    runTime.timeName(),
    mesh
  },
  mixture.rho()
};
// Mass flux
surfaceScalarField rhoPhi
{
  {
    "rhoPhi",
    runTime.timeName(),
    mesh
  },
  fvc::interpolate(rho)*phi
};
// Relative Velocity
autoPtr<relativeVelocityModel> UdmModelPtr
{
  relativeVelocityModel::New
  (
    mixture,
    mixture
  )
};
relativeVelocityModel& UdmModel = UdmModelPtr();
// Construct compressible turbulence model
autoPtr
<
  CompressibleTurbulenceModel<incompressibleTwoPhaseInteractingMixture>
> turbulence
{
  CompressibleTurbulenceModel<incompressibleTwoPhaseInteractingMixture>::New
  (
    rho,
    U,
    rhoPhi,
    mixture
  )
};
#include "read_gravitational_acceleration.inc"
#include "read_href.inc"
#include "gh.inc"
volScalarField p
{
  {
    "p",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ,
    IOobject::AUTO_WRITE
  },
  p_rgh + rho*gh
};
label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell
(
  p,
  p_rgh,
  pimple.dict(),
  pRefCell,
  pRefValue
);
if (p_rgh.needReference())
{
  p += dimensionedScalar
  {
    "p",
    p.dimensions(),
    pRefValue - getRefCellValue(p, pRefCell)
  };
  p_rgh = p - rho*gh;
}
mesh.setFluxRequired(p_rgh.name());
mesh.setFluxRequired(alpha1.name());
// MULES Correction
tmp<surfaceScalarField> talphaPhiCorr0;
