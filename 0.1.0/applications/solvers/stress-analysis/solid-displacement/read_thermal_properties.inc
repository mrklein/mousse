Info<< "Reading thermal properties\n" << endl;
IOdictionary thermalProperties
{
  {
    "thermalProperties",
    runTime.constant(),
    mesh,
    IOobject::MUST_READ_IF_MODIFIED
  }
};
Switch thermalStress{thermalProperties.lookup("thermalStress")};
volScalarField threeKalpha
{
  {
    "threeKalpha",
    runTime.timeName(),
    mesh
  },
  mesh,
  {"0", dimensionSet(0, 2, -2 , -1, 0), 0.0}
};
volScalarField DT
{
  {
    "DT",
    runTime.timeName(),
    mesh
  },
  mesh,
  {"0", dimensionSet(0, 2, -1 , 0, 0), 0.0}
};
if (thermalStress)
{
  autoPtr<volScalarField> CPtr;
  IOobject CIO
  {
    "C",
    runTime.timeName(0),
    mesh
  };
  const dictionary& CDict = thermalProperties.subDict("C");
  word CType{CDict.lookup("type")};
  if (CType == "uniform")
  {
    scalar CValue{readScalar(CDict.lookup("value"))};
    CPtr.reset
    (
      new volScalarField
      {
        CIO,
        mesh,
        {"C", dimensionSet(0, 2, -2 , -1, 0), CValue},
        zeroGradientFvPatchField<scalar>::typeName
      }
    );
  }
  else if (CType == "field")
  {
    CIO.readOpt() = IOobject::MUST_READ;
    CPtr.reset
    (
      new volScalarField
      {
        CIO,
        mesh
      }
    );
  }
  else
  {
    FATAL_ERROR_IN
    (
      "readThermalProperties.H"
    )
    << "Valid type entries are uniform or field for C"
    << abort(FatalError);
  }
  volScalarField& C = CPtr();
  autoPtr<volScalarField> rhoKPtr;
  IOobject rhoKIO
  {
    "k",
    runTime.timeName(0),
    mesh
  };
  const dictionary& kDict = thermalProperties.subDict("k");
  word kType{kDict.lookup("type")};
  if (kType == "uniform")
  {
    scalar rhoKValue{readScalar(kDict.lookup("value"))};
    rhoKPtr.reset
    (
      new volScalarField
      {
        rhoKIO,
        mesh,
        {"rhoK", dimensionSet(1, 1, -3 , -1, 0), rhoKValue},
        zeroGradientFvPatchField<scalar>::typeName
      }
    );
  }
  else if (kType == "field")
  {
    rhoKIO.readOpt() = IOobject::MUST_READ;
    rhoKPtr.reset
    (
      new volScalarField
      {
        rhoKIO,
        mesh
      }
    );
  }
  else
  {
    FATAL_ERROR_IN
    (
      "readThermalProperties.H"
    )
    << "Valid type entries are uniform or field for K"
    << abort(FatalError);
  }
  volScalarField& rhoK = rhoKPtr();
  autoPtr<volScalarField> alphaPtr;
  IOobject alphaIO
  {
    "alpha",
    runTime.timeName(0),
    mesh
  };
  const dictionary& alphaDict = thermalProperties.subDict("alpha");
  word alphaType{alphaDict.lookup("type")};
  if (alphaType == "uniform")
  {
    scalar alphaValue{readScalar(alphaDict.lookup("value"))};
    alphaPtr.reset
    (
      new volScalarField
      {
        alphaIO,
        mesh,
        {"alpha", inv(dimTemperature), alphaValue},
        zeroGradientFvPatchField<scalar>::typeName
      }
    );
  }
  else if (alphaType == "field")
  {
    alphaIO.readOpt() = IOobject::MUST_READ;
    alphaPtr.reset
    (
      new volScalarField
      {
        alphaIO,
        mesh
      }
    );
  }
  else
  {
    FATAL_ERROR_IN
    (
      "readThermalProperties.H"
    )
    << "Valid type entries are uniform or field for alpha"
    << abort(FatalError);
  }
  volScalarField& alpha = alphaPtr();
  Info<< "Normalising k : k/rho\n" << endl;
  volScalarField k{rhoK/rho};
  Info<< "Calculating thermal coefficients\n" << endl;
  threeKalpha = threeK*alpha;
  DT = k/C;
}
