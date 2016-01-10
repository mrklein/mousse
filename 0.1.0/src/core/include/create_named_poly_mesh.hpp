//
// createNamedPolyMesh.H
// ~~~~~~~~~~~~~~~~~~~~~
  mousse::word regionName;
  if (args.optionReadIfPresent("region", regionName))
  {
    mousse::Info
      << "Create polyMesh " << regionName << " for time = "
      << runTime.timeName() << mousse::nl << mousse::endl;
  }
  else
  {
    regionName = mousse::polyMesh::defaultRegion;
    mousse::Info
      << "Create polyMesh for time = "
      << runTime.timeName() << mousse::nl << mousse::endl;
  }
  mousse::polyMesh mesh
  (
    mousse::IOobject
    (
      regionName,
      runTime.timeName(),
      runTime,
      mousse::IOobject::MUST_READ
    )
  );
