  if (mesh.nCells() != 1) {
    FATAL_ERROR_IN_FUNCTION
      << "Solver only applicable to single cell cases"
      << exit(FatalError);
  }
  Info << "Reading initial conditions.\n" << endl;
  IOdictionary initialConditions
  {
    {
      "initialConditions",
      runTime.constant(),
      runTime,
      IOobject::MUST_READ_IF_MODIFIED
    }
  };
  scalar p0 = readScalar(initialConditions.lookup("p"));
  scalar T0 = readScalar(initialConditions.lookup("T"));
  #include "create_base_fields.inc"
  Info << nl << "Reading thermophysicalProperties" << endl;
  autoPtr<psiChemistryModel> pChemistry{psiChemistryModel::New(mesh)};
  psiChemistryModel& chemistry = pChemistry();
  scalar dtChem = refCast<const psiChemistryModel>(chemistry).deltaTChem()[0];
  psiReactionThermo& thermo = chemistry.thermo();
  thermo.validate(args.executable(), "h");
  basicMultiComponentMixture& composition = thermo.composition();
  PtrList<volScalarField>& Y = composition.Y();
  volScalarField rho
  {
    {
      "rho",
      runTime.timeName(),
      runTime,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    thermo.rho()
  };
  volScalarField& p = thermo.p();
  volScalarField Rspecific
  {
    {
      "Rspecific",
      runTime.timeName(),
      runTime,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    mesh,
    {"zero", dimensionSet(dimEnergy/dimMass/dimTemperature), 0.0}
  };
  volVectorField U
  {
    {
      "U",
      runTime.timeName(),
      runTime
    },
    mesh,
    {"zero", dimVelocity, vector::zero},
    p.boundaryField().types()
  };
  #include "create_phi.inc"
  OFstream post{args.path()/"chemFoam.out"};
  post << "# Time" << token::TAB << "Temperature [K]" << token::TAB
    << "Pressure [Pa]" << endl;
