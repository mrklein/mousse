  Info << "Reading thermophysical properties\n" << endl;
  autoPtr<psiThermo> pThermo
  {
    psiThermo::New(mesh)
  };
  psiThermo& thermo = pThermo();
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
    compressible::turbulenceModel::New
    (
      rho,
      U,
      phi,
      thermo
    )
  };
  const word kinematicCloudName
  {
    args.optionLookupOrDefault<word>("cloudName", "kinematicCloud")
  };
  Info << "Constructing kinematicCloud " << kinematicCloudName << endl;
  basicKinematicCloud kinematicCloud
  {
    kinematicCloudName,
    rho,
    U,
    thermo.mu(),
    g
  };
