Info << "Creating combustion model\n" << endl;
autoPtr<combustionModels::psiCombustionModel> combustion
{
  combustionModels::psiCombustionModel::New(mesh)
};
Info << "Reading thermophysical properties\n" << endl;
psiReactionThermo& thermo = combustion->thermo();
thermo.validate(args.executable(), "h", "e");
SLGThermo slgThermo{mesh, thermo};
basicMultiComponentMixture& composition = thermo.composition();
PtrList<volScalarField>& Y = composition.Y();
const word inertSpecie{thermo.lookup("inertSpecie")};
Info << "Creating field rho\n" << endl;
volScalarField rho
{
  {
    "rho",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ,
    IOobject::AUTO_WRITE
  },
  thermo.rho()
};
volScalarField& p = thermo.p();
const volScalarField& T = thermo.T();
const volScalarField& psi = thermo.psi();
Info << "\nReading field U\n" << endl;
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
// Set the turbulence into the combustion model
combustion->setTurbulence(turbulence());
volScalarField dQ
{
  {
    "dQ",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ,
    IOobject::AUTO_WRITE
  },
  mesh,
  {"dQ", dimEnergy/dimTime, 0.0}
};
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
#include "read_gravitational_acceleration.inc"
#include "read_href.inc"
#include "gh.inc"
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
multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields;
FOR_ALL(Y, i) {
  fields.add(Y[i]);
}
fields.add(thermo.he());
IOdictionary additionalControlsDict
{
  {
    "additionalControls",
    runTime.constant(),
    mesh,
    IOobject::MUST_READ_IF_MODIFIED
  }
};
Switch solvePrimaryRegion{additionalControlsDict.lookup("solvePrimaryRegion")};
