  Info << "Reading field T\n" << endl;
  volScalarField T
  {
    {
      "T",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    mesh
  };
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
  Info << "Reading transportProperties\n" << endl;
  IOdictionary transportProperties
  {
    {
      "transportProperties",
      runTime.constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  };
  Info << "Reading diffusivity DT\n" << endl;
  dimensionedScalar DT{transportProperties.lookup("DT")};
  #include "create_phi.inc"

