Info << "Reading thermophysical properties\n" << endl;
autoPtr<rhoThermo> pThermo{rhoThermo::New(mesh)};
rhoThermo& thermo = pThermo();
thermo.validate(args.executable(), "h", "e");
volScalarField rho
{
  {
    "rho",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ,
  },
  thermo.rho()
};
volScalarField& p = thermo.p();
const volScalarField& psi = thermo.psi();
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
#include "compressible_create_phi.inc"
Info << "Creating turbulence model\n" << endl;
autoPtr<compressible::turbulenceModel> turbulence
{
  compressible::turbulenceModel::New(rho, U, phi, thermo)
};
#include "read_gravitational_acceleration.inc"
#include "read_href.inc"
#include "gh.inc"
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
// Force p_rgh to be consistent with p
p_rgh = p - rho*gh;
mesh.setFluxRequired(p_rgh.name());
Info << "Creating field dpdt\n" << endl;
volScalarField dpdt
{
  {
    "dpdt",
    runTime.timeName(),
    mesh
  },
  mesh,
  {"dpdt", p.dimensions()/dimTime, 0}
};
Info << "Creating field kinetic energy K\n" << endl;
volScalarField K{"K", 0.5*magSqr(U)};
