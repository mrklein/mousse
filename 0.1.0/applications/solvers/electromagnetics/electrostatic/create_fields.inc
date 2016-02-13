  Info<< "Reading physicalProperties\n" << endl;
  IOdictionary physicalProperties
  {
    {
      "physicalProperties",
      runTime.constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED
    }
  };
  dimensionedScalar epsilon0
  {
    physicalProperties.lookup("epsilon0")
  };
  dimensionedScalar k
  {
    physicalProperties.lookup("k")
  };
  Info<< "Reading field phi\n" << endl;
  volScalarField phi
  {
    {
      "phi",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    mesh
  };
  Info<< "Reading field rho\n" << endl;
  volScalarField rho
  {
    {
      "rho",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    mesh
  };
  Info<< "Calculating field rhoFlux\n" << endl;
  surfaceScalarField rhoFlux
  {
    {
      "rhoFlux",
      runTime.timeName(),
      mesh,
      IOobject::NO_READ
    },
    -k*mesh.magSf()*fvc::snGrad(phi)
  };
