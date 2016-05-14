  Info << nl << "Reading transportProperties" << endl;
  IOdictionary transportProperties
  {
    {
      "transportProperties",
      runTime.constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED
    }
  };
