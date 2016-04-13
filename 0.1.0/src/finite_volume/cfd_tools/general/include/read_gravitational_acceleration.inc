  Info << "\nReading g" << endl;
  uniformDimensionedVectorField g
  {
    {
      "g",
      runTime.constant(),
      mesh,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    }
  };
