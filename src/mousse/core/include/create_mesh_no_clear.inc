// create_mesh_no_clear.inc
// currently identical to create_mesh.inc
  mousse::Info
    << "Create mesh, no clear-out for time = "
    << runTime.timeName() << mousse::nl << mousse::endl;
  mousse::fvMesh mesh
  {
    // mousse::IOobject
    {
      mousse::fvMesh::defaultRegion,
      runTime.timeName(),
      runTime,
      mousse::IOobject::MUST_READ
    }
  };
