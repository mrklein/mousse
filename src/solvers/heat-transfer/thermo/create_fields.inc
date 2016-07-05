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
      IOobject::NO_WRITE
    },
    thermo.rho()
  };
  volScalarField& p = thermo.p();
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
  #include "set_alpha_eff.inc"
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