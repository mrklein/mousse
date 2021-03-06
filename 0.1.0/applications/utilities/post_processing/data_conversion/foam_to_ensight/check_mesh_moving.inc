// check for "points" in any of the result directories
bool meshMoving = false;
if (Times.size() > 1) {
  // We already loaded a mesh (usually from constant). See if any other
  // points files
  FOR_ALL(Times, timeI) {
    if (Times[timeI].name() == mesh.pointsInstance())
      continue;
    IOobject io
    {
      "points",
      Times[timeI].name(),
      polyMesh::meshSubDir,
      mesh,
      IOobject::NO_READ
    };
    if (io.headerOk()) {
      meshMoving = true;
      break;
    }
  }
}

