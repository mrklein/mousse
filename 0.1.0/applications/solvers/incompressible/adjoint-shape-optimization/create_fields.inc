Info << "Reading field p\n" << endl;
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
label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p, simple.dict(), pRefCell, pRefValue);
mesh.setFluxRequired(p.name());
Info << "Reading field pa\n" << endl;
volScalarField pa
{
  {
    "pa",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
Info << "Reading field Ua\n" << endl;
volVectorField Ua
{
  {
    "Ua",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
#include "create_phia.inc"
label paRefCell = 0;
scalar paRefValue = 0.0;
setRefCell
(
  pa,
  simple.dict(),
  paRefCell,
  paRefValue
);
mesh.setFluxRequired(pa.name());
singlePhaseTransportModel laminarTransport{U, phi};
autoPtr<incompressible::turbulenceModel> turbulence
{
  incompressible::turbulenceModel::New(U, phi, laminarTransport)
};
dimensionedScalar zeroSensitivity{"0", dimVelocity*dimVelocity, 0.0};
dimensionedScalar zeroAlpha{"0", dimless/dimTime, 0.0};
dimensionedScalar lambda{laminarTransport.lookup("lambda")};
dimensionedScalar alphaMax{laminarTransport.lookup("alphaMax")};
const labelList& inletCells = mesh.boundary()["inlet"].faceCells();
//const labelList& outletCells = mesh.boundary()["outlet"].faceCells();
volScalarField alpha
{
  {
    "alpha",
    runTime.timeName(),
    mesh,
    IOobject::READ_IF_PRESENT,
    IOobject::AUTO_WRITE
  },
  lambda*max(Ua & U, zeroSensitivity)
};
zeroCells(alpha, inletCells);
