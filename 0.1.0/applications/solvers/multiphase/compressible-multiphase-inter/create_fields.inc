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
Info<< "Constructing multiphaseMixtureThermo\n" << endl;
multiphaseMixtureThermo mixture{U, phi};
volScalarField& p = mixture.p();
volScalarField& T = mixture.T();
volScalarField rho
{
  {
    "rho",
    runTime.timeName(),
    mesh,
    IOobject::READ_IF_PRESENT
  },
  mixture.rho()
};
dimensionedScalar pMin{"pMin", dimPressure, mixture};
mesh.setFluxRequired(p_rgh.name());
#include "read_gravitational_acceleration.inc"
#include "read_href.inc"
#include "gh.inc"
// Construct compressible turbulence model
autoPtr<compressible::turbulenceModel> turbulence
{
  compressible::turbulenceModel::New
  (
    rho,
    U,
    mixture.rhoPhi(),
    mixture
  )
};
Info<< "Creating field kinetic energy K\n" << endl;
volScalarField K{"K", 0.5*magSqr(U)};
