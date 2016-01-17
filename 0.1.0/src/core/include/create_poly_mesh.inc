// create_poly_mesh.inc
  mousse::Info
    << "Create polyMesh for time = "
    << runTime.timeName() << mousse::nl << mousse::endl;
  mousse::polyMesh mesh
  {
    // mousse::IOobject
    {
      mousse::polyMesh::defaultRegion,
      runTime.timeName(),
      runTime,
      mousse::IOobject::MUST_READ
    }
  };
