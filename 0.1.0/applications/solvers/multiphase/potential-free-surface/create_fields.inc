Info << "Reading field p (kinematic)\n" << endl;
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
singlePhaseTransportModel laminarTransport{U, phi};
autoPtr<incompressible::turbulenceModel> turbulence
{
  incompressible::turbulenceModel::New(U, phi, laminarTransport)
};
#include "read_gravitational_acceleration.inc"
Info << "Creating field zeta\n" << endl;
volVectorField zeta
{
  {
    "zeta",
    runTime.timeName(),
    mesh,
    IOobject::READ_IF_PRESENT,
    IOobject::AUTO_WRITE
  },
  mesh,
  {"zero", dimLength, vector::zero}
};
Info << "Creating field p_gh\n" << endl;
volScalarField p_gh
{
  {
    "p_gh",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
// Force p_gh to be consistent with p
// Height is made relative to field 'refLevel'
p_gh =  p - (g & mesh.C());
label p_ghRefCell = 0;
scalar p_ghRefValue = 0.0;
setRefCell(p_gh, pimple.dict(), p_ghRefCell, p_ghRefValue);
mesh.setFluxRequired(p_gh.name());

