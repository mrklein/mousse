Info<< "Reading thermophysical properties\n" << endl;
autoPtr<psiThermo> pThermo
{
  psiThermo::New(mesh)
};
psiThermo& thermo = pThermo();
volScalarField& p = thermo.p();
volScalarField& e = thermo.he();
const volScalarField& T = thermo.T();
const volScalarField& psi = thermo.psi();
const volScalarField& mu = thermo.mu();
bool inviscid(true);
if (max(mu.internalField()) > 0.0)
{
  inviscid = false;
}
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
volVectorField rhoU
{
  {
    "rhoU",
    runTime.timeName(),
    mesh
  },
  rho*U
};
volScalarField rhoE
{
  {
    "rhoE",
    runTime.timeName(),
    mesh
  },
  rho*(e + 0.5*magSqr(U))
};
surfaceScalarField pos
{
  {
    "pos",
    runTime.timeName(),
    mesh
  },
  mesh,
  {"pos", dimless, 1.0}
};
surfaceScalarField neg
{
  {
    "neg",
    runTime.timeName(),
    mesh
  },
  mesh,
  {"neg", dimless, -1.0}
};
surfaceScalarField phi{"phi", mesh.Sf() & fvc::interpolate(rhoU)};
Info<< "Creating turbulence model\n" << endl;
autoPtr<compressible::turbulenceModel> turbulence
{
  compressible::turbulenceModel::New
  (
    rho,
    U,
    phi,
    thermo
  )
};
