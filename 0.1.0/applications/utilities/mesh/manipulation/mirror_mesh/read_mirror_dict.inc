  Info << "Reading mirrorMeshDict\n" << endl;
  IOdictionary mirrorMeshDict
  {
    {
      "mirrorMeshDict",
      runTime.system(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED
    }
  };
  plane mirrorPlane{mirrorMeshDict};
  scalar planeTolerance{readScalar(mirrorMeshDict.lookup("planeTolerance"))};
