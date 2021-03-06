  Info << "Reading thermophysical properties\n" << endl;
  autoPtr<psiThermo> pThermo{psiThermo::New(mesh)};
  psiThermo& thermo = pThermo();
  thermo.validate(args.executable(), "h", "e");
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
  const volScalarField& T = thermo.T();
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

