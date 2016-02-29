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
Info << "Creating phaseChangeTwoPhaseMixture\n" << endl;
autoPtr<phaseChangeTwoPhaseMixture> mixture =
  phaseChangeTwoPhaseMixture::New(U, phi);
volScalarField& alpha1{mixture->alpha1()};
volScalarField& alpha2{mixture->alpha2()};
const dimensionedScalar& rho1 = mixture->rho1();
const dimensionedScalar& rho2 = mixture->rho2();
const dimensionedScalar& pSat = mixture->pSat();
// Need to store rho for ddt(rho, U)
volScalarField rho
{
  {
    "rho",
    runTime.timeName(),
    mesh,
    IOobject::READ_IF_PRESENT
  },
  alpha1*rho1 + alpha2*rho2,
  alpha1.boundaryField().types()
};
rho.oldTime();
// Construct interface from alpha1 distribution
interfaceProperties interface{alpha1, U, mixture()};
// Construct incompressible turbulence model
autoPtr<incompressible::turbulenceModel> turbulence
{
  incompressible::turbulenceModel::New(U, phi, mixture())
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
