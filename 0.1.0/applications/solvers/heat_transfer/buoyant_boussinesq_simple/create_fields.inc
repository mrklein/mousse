Info<< "Reading thermophysical properties\n" << endl;
Info<< "Reading field T\n" << endl;
volScalarField T
{
  {
    "T",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
Info<< "Reading field p_rgh\n" << endl;
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
Info<< "Reading field U\n" << endl;
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
#include "read_transport_properties.inc"
Info<< "Creating turbulence model\n" << endl;
autoPtr<incompressible::turbulenceModel> turbulence
{
  incompressible::turbulenceModel::New(U, phi, laminarTransport)
};
// Kinematic density for buoyancy force
volScalarField rhok
{
  {
    "rhok",
    runTime.timeName(),
    mesh
  },
  1.0 - beta*(T - TRef)
};
// kinematic turbulent thermal thermal conductivity m2/s
Info<< "Reading field alphat\n" << endl;
volScalarField alphat
{
  {
    "alphat",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
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
  p_rgh + rhok*gh
};
label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell
(
  p,
  p_rgh,
  simple.dict(),
  pRefCell,
  pRefValue
);
if (p_rgh.needReference())
{
  p += dimensionedScalar
  (
    "p",
    p.dimensions(),
    pRefValue - getRefCellValue(p, pRefCell)
  );
}
mesh.setFluxRequired(p_rgh.name());
