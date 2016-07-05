  mousse::word regionName;
  if (args.optionReadIfPresent("region", regionName)) {
    mousse::Info << "Create mesh " << regionName << " for time = "
      << runTime.timeName() << mousse::nl << mousse::endl;
  } else {
    regionName = mousse::fvMesh::defaultRegion;
    mousse::Info << "Create mesh for time = "
      << runTime.timeName() << mousse::nl << mousse::endl;
  }
  autoPtr<dynamicFvMesh> meshPtr
  {
    dynamicFvMesh::New
    (
      {
        regionName,
        runTime.timeName(),
        runTime,
        IOobject::MUST_READ
      }
    )
  };
  dynamicFvMesh& mesh = meshPtr();
