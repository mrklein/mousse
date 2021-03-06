Info << "Reading thermophysical properties\n" << endl;
autoPtr<psiuReactionThermo> pThermo{psiuReactionThermo::New(mesh)};
psiuReactionThermo& thermo = pThermo();
thermo.validate(args.executable(), "ha", "ea");
basicMultiComponentMixture& composition = thermo.composition();
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
const volScalarField& psi = thermo.psi();
volScalarField& b = composition.Y("b");
Info << "min(b) = " << min(b).value() << endl;
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
mesh.setFluxRequired(p.name());
Info << "Creating turbulence model\n" << endl;
autoPtr<compressible::RASModel> turbulence
{
  compressible::New<compressible::RASModel>
  (
    rho,
    U,
    phi,
    thermo
  )
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
Info << "Creating the unstrained laminar flame speed\n" << endl;
autoPtr<laminarFlameSpeed> unstrainedLaminarFlameSpeed
{
  laminarFlameSpeed::New(thermo)
};
Info << "Reading strained laminar flame speed field Su\n" << endl;
volScalarField Su
{
  {
    "Su",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
Info<< "Reading field betav\n" << endl;
volScalarField betav
{
  {
    "betav",
    mesh.facesInstance(),
    mesh,
    IOobject::MUST_READ
  },
  mesh
};
Info << "Reading field Lobs\n" << endl;
volScalarField Lobs
{
  {
    "Lobs",
    mesh.facesInstance(),
    mesh,
    IOobject::MUST_READ
  },
  mesh
};
Info << "Reading field CT\n" << endl;
volSymmTensorField CT
{
  {
    "CT",
    mesh.facesInstance(),
    mesh,
    IOobject::MUST_READ
  },
  mesh
};
Info << "Reading field Nv\n" << endl;
volScalarField Nv
{
  {
    "Nv",
    mesh.facesInstance(),
    mesh,
    IOobject::MUST_READ
  },
  mesh
};
Info<< "Reading field nsv\n" << endl;
volSymmTensorField nsv
{
  {
    "nsv",
    mesh.facesInstance(),
    mesh,
    IOobject::MUST_READ
  },
  mesh
};
IOdictionary PDRProperties
{
  {
    "PDRProperties",
    runTime.constant(),
    mesh,
    IOobject::MUST_READ_IF_MODIFIED
  }
};
//- Create the drag model
autoPtr<PDRDragModel> drag = PDRDragModel::New
(
  PDRProperties,
  turbulence,
  rho,
  U,
  phi
);
//- Create the flame-wrinkling model
autoPtr<XiModel> flameWrinkling = XiModel::New
(
  PDRProperties,
  thermo,
  turbulence,
  Su,
  rho,
  b,
  phi
);
Info << "Calculating turbulent flame speed field St\n" << endl;
volScalarField St
{
  {
    "St",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ,
    IOobject::AUTO_WRITE
  },
  flameWrinkling->Xi()*Su
};
multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields;
if (composition.contains("ft"))
{
  fields.add(composition.Y("ft"));
}
fields.add(b);
fields.add(thermo.he());
fields.add(thermo.heu());
flameWrinkling->addXi(fields);

