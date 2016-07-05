  Info << "Reading mechanical properties\n" << endl;
  IOdictionary mechanicalProperties
  {
    {
      "mechanicalProperties",
      runTime.constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED
    }
  };
  const dictionary& rhoDict = mechanicalProperties.subDict("rho");
  word rhoType{rhoDict.lookup("type")};
  autoPtr<volScalarField> rhoPtr;
  IOobject rhoIO
  {
    "rho",
    runTime.timeName(0),
    mesh
  };
  if (rhoType == "uniform") {
    scalar rhoValue{readScalar(rhoDict.lookup("value"))};
    rhoPtr.reset
    (
      new volScalarField
      {
        rhoIO,
        mesh,
        {"rho", dimMass/dimVolume, rhoValue},
        zeroGradientFvPatchField<scalar>::typeName
      }
    );
  } else if (rhoType == "field") {
    rhoIO.readOpt() = IOobject::MUST_READ;
    rhoPtr.reset
    (
      new volScalarField{rhoIO, mesh}
    );
  } else {
    FATAL_ERROR_IN
    (
      "readMechanicalProperties.H"
    )
    << "Valid type entries are uniform or field for rho"
    << abort(FatalError);
  }
  volScalarField& rho = rhoPtr();
  const dictionary& EDict = mechanicalProperties.subDict("E");
  word EType{EDict.lookup("type")};
  autoPtr<volScalarField> EPtr;
  IOobject EHeader
  {
    "E",
    runTime.timeName(0),
    mesh
  };
  if (EType == "uniform") {
    scalar rhoEValue{readScalar(EDict.lookup("value"))};
    EPtr.reset
    (
      new volScalarField
      {
        EHeader,
        mesh,
        {"Erho", dimMass/dimLength/sqr(dimTime), rhoEValue},
        zeroGradientFvPatchField<scalar>::typeName
      }
    );
  } else if (EType == "field") {
    EHeader.readOpt() = IOobject::MUST_READ;
    EPtr.reset
    (
      new volScalarField{EHeader, mesh}
    );
  } else {
    FATAL_ERROR_IN
    (
      "readMechanicalProperties.H"
    )
    << "Valid type entries are uniform or field for E"
    << abort(FatalError);
  }
  volScalarField& rhoE = EPtr();
  autoPtr<volScalarField> nuPtr;
  IOobject nuIO
  {
    "nu",
    runTime.timeName(0),
    mesh
  };
  const dictionary& nuDict = mechanicalProperties.subDict("nu");
  word nuType{nuDict.lookup("type")};
  if (nuType == "uniform") {
    scalar nuValue{readScalar(nuDict.lookup("value"))};
    nuPtr.reset
    (
      new volScalarField
      {
        nuIO,
        mesh,
        {"nu", dimless, nuValue},
        zeroGradientFvPatchField<scalar>::typeName
      }
    );
  } else if (nuType == "field") {
    nuIO.readOpt() = IOobject::MUST_READ;
    nuPtr.reset
    (
      new volScalarField{nuIO, mesh}
    );
  } else {
    FATAL_ERROR_IN
    (
      "readMechanicalProperties.H"
    )
    << "Valid type entries are uniform or field for nu"
    << abort(FatalError);
  }
  volScalarField& nu = nuPtr();
  Info << "Normalising E : E/rho\n" << endl;
  volScalarField E{rhoE/rho};
  Info << "Calculating Lame's coefficients\n" << endl;
  volScalarField mu{E/(2.0*(1.0 + nu))};
  volScalarField lambda{nu*E/((1.0 + nu)*(1.0 - 2.0*nu))};
  volScalarField threeK{E/(1.0 - 2.0*nu)};
  Switch planeStress{mechanicalProperties.lookup("planeStress")};
  if (planeStress) {
    Info << "Plane Stress\n" << endl;
    lambda = nu*E/((1.0 + nu)*(1.0 - nu));
    threeK = E/(1.0 - nu);
  } else {
    Info << "Plane Strain\n" << endl;
  }