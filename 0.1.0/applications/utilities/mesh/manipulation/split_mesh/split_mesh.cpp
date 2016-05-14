// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "poly_mesh.hpp"
#include "time.hpp"
#include "poly_topo_change.hpp"
#include "map_poly_mesh.hpp"
#include "face_set.hpp"
#include "attach_detach.hpp"
#include "attach_poly_topo_changer.hpp"
#include "region_side.hpp"
#include "primitive_face_patch.hpp"


using namespace mousse;


// Find edge between points v0 and v1.
label findEdge(const primitiveMesh& mesh, const label v0, const label v1)
{
  const labelList& pEdges = mesh.pointEdges()[v0];
  FOR_ALL(pEdges, pEdgeI) {
    label edgeI = pEdges[pEdgeI];
    const edge& e = mesh.edges()[edgeI];
    if (e.otherVertex(v0) == v1) {
      return edgeI;
    }
  }
  FATAL_ERROR_IN
  (
    "findEdge(const primitiveMesh&, const label, const label)"
  )
  << "Cannot find edge between mesh points " << v0 << " and " << v1
  << abort(FatalError);
  return -1;
}


// Checks whether patch present
void checkPatch(const polyBoundaryMesh& bMesh, const word& name)
{
  const label patchI = bMesh.findPatchID(name);
  if (patchI == -1) {
    FATAL_ERROR_IN("checkPatch(const polyBoundaryMesh&, const word&)")
      << "Cannot find patch " << name << nl
      << "It should be present but of zero size" << endl
      << "Valid patches are " << bMesh.names()
      << exit(FatalError);
  }
  if (bMesh[patchI].size()) {
    FATAL_ERROR_IN("checkPatch(const polyBoundaryMesh&, const word&)")
      << "Patch " << name << " is present but non-zero size"
      << exit(FatalError);
  }
}


int main(int argc, char *argv[])
{
  argList::noParallel();
  #include "add_overwrite_option.inc"
  argList::validArgs.append("faceSet");
  argList::validArgs.append("masterPatch");
  argList::validArgs.append("slavePatch");
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  #include "create_poly_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  const word setName = args[1];
  const word masterPatch = args[2];
  const word slavePatch = args[3];
  const bool overwrite = args.optionFound("overwrite");
  // List of faces to split
  faceSet facesSet{mesh, setName};
  Info << "Read " << facesSet.size() << " faces to split" << endl << endl;
  // Convert into labelList and check
  labelList faces{facesSet.toc()};
  FOR_ALL(faces, i) {
    if (!mesh.isInternalFace(faces[i])) {
      FATAL_ERROR_IN(args.executable())
        << "Face " << faces[i] << " in faceSet " << setName
        << " is not an internal face."
        << exit(FatalError);
    }
  }
  // Check for empty master and slave patches
  checkPatch(mesh.boundaryMesh(), masterPatch);
  checkPatch(mesh.boundaryMesh(), slavePatch);
  //
  // Find 'side' of all faces on splitregion. Uses regionSide which needs
  // set of edges on side of this region. Use PrimitivePatch to find these.
  //
  // Addressing on faces only in mesh vertices.
  primitiveFacePatch fPatch
  {
    faceList{UIndirectList<face>{mesh.faces(), faces}},
    mesh.points()
  };
  const labelList& meshPoints = fPatch.meshPoints();
  // Mark all fence edges : edges on boundary of fPatch but not on boundary
  // of polyMesh
  labelHashSet fenceEdges{fPatch.size()};
  const labelListList& allEdgeFaces = fPatch.edgeFaces();
  FOR_ALL(allEdgeFaces, patchEdgeI) {
    if (allEdgeFaces[patchEdgeI].size() == 1) {
      const edge& e = fPatch.edges()[patchEdgeI];
      label edgeI =
        findEdge
        (
          mesh,
          meshPoints[e.start()],
          meshPoints[e.end()]
        );
      fenceEdges.insert(edgeI);
    }
  }
  // Find sides reachable from 0th face of faceSet
  label startFaceI = faces[0];
  regionSide regionInfo
  {
    mesh,
    facesSet,
    fenceEdges,
    mesh.faceOwner()[startFaceI],
    startFaceI
  };
  // Determine flip state for all faces in faceSet
  boolList zoneFlip{faces.size()};
  FOR_ALL(faces, i) {
    zoneFlip[i] = !regionInfo.sideOwner().found(faces[i]);
  }
  // Create and add face zones and mesh modifiers
  List<pointZone*> pz{0};
  List<faceZone*> fz{1};
  List<cellZone*> cz{0};
  fz[0] =
    new faceZone
    {
      "membraneFaces",
      faces,
      zoneFlip,
      0,
      mesh.faceZones()
    };
  Info << "Adding point and face zones" << endl;
  mesh.addZones(pz, fz, cz);
  attachPolyTopoChanger splitter{mesh};
  splitter.setSize(1);
  // Add the sliding interface mesh modifier to start working at current
  // time
  splitter.set
  (
    0,
    new attachDetach
    {
      "Splitter",
      0,
      splitter,
      "membraneFaces",
      masterPatch,
      slavePatch,
      scalarField(1, runTime.value())
    }
  );
  Info << nl << "Constructed topologyModifier:" << endl;
  splitter[0].writeDict(Info);
  if (!overwrite) {
    runTime++;
  }
  splitter.attach();
  if (overwrite) {
    mesh.setInstance(oldInstance);
  } else {
    mesh.setInstance(runTime.timeName());
  }
  Info << "Writing mesh to " << runTime.timeName() << endl;
  if (!mesh.write()) {
    FATAL_ERROR_IN(args.executable())
      << "Failed writing polyMesh."
      << exit(FatalError);
  }
  Info << nl << "end" << endl;
  return 0;
}

