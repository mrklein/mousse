Info<< "Reading transportProperties\n" << endl;
IOdictionary transportProperties
{
  {
    "transportProperties",
    runTime.constant(),
    mesh,
    IOobject::MUST_READ_IF_MODIFIED
  }
};
dimensionedScalar rho
{
  "rho",
  dimDensity,
  transportProperties
};
dimensionedScalar nu
{
  "nu",
  dimViscosity,
  transportProperties
};
dimensionedScalar mu
{
  "mu",
  dimensionSet(1, 1, -2, 0, 0, -2, 0),
  transportProperties
};
dimensionedScalar sigma
{
  "sigma",
  dimensionSet(-1, -3, 3, 0, 0, 2, 0),
  transportProperties
};
Info<< "Reading field p\n" << endl;
volScalarField p
{
  {
    "p",
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
Info<< "Reading field pB\n" << endl;
volScalarField pB
{
  {
    "pB",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
Info<< "Reading field B\n" << endl;
volVectorField B
{
  {
    "B",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
#include "create_phi_b.inc"
dimensionedScalar DB = 1.0/(mu*sigma);
DB.name() = "DB";
dimensionedScalar DBU = 1.0/(2.0*mu*rho);
DBU.name() = "DBU";
label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p, piso.dict(), pRefCell, pRefValue);
mesh.setFluxRequired(p.name());
mesh.setFluxRequired(pB.name());
