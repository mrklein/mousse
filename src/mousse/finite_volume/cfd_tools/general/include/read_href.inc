  Info << "\nReading hRef" << endl;
  uniformDimensionedScalarField hRef
  {
    {
      "hRef",
      runTime.constant(),
      mesh,
      IOobject::READ_IF_PRESENT
    },
    {"hRef", dimLength, 0}
  };
