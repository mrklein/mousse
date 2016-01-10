//
// createNamedMesh.H
// ~~~~~~~~~~~~~~~~~
  mousse::word regionName;
  if (args.optionReadIfPresent("region", regionName))
  {
    mousse::Info
      << "Create mesh " << regionName << " for time = "
      << runTime.timeName() << mousse::nl << mousse::endl;
  }
  else
  {
    regionName = mousse::fvMesh::defaultRegion;
    mousse::Info
      << "Create mesh for time = "
      << runTime.timeName() << mousse::nl << mousse::endl;
  }
  mousse::fvMesh mesh
  (
    mousse::IOobject
    (
      regionName,
      runTime.timeName(),
      runTime,
      mousse::IOobject::MUST_READ
    )
  );
