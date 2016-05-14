// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "poly_mesh.hpp"
#include "time.hpp"
#include "boundary_mesh.hpp"
#include "repatch_poly_topo_changer.hpp"
#include "unit_conversion.hpp"
#include "ofstream.hpp"
#include "list_ops.hpp"


using namespace mousse;


// Get all feature edges.
void collectFeatureEdges(const boundaryMesh& bMesh, labelList& markedEdges)
{
  markedEdges.setSize(bMesh.mesh().nEdges());
  label markedI = 0;
  FOR_ALL(bMesh.featureSegments(), i) {
    const labelList& segment = bMesh.featureSegments()[i];
    FOR_ALL(segment, j) {
      label featEdgeI = segment[j];
      label meshEdgeI = bMesh.featureToEdge()[featEdgeI];
      markedEdges[markedI++] = meshEdgeI;
    }
  }
  markedEdges.setSize(markedI);
}


int main(int argc, char *argv[])
{
  #include "add_overwrite_option.inc"
  argList::noParallel();
  argList::validArgs.append("feature angle[0-180]");
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  #include "create_poly_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  Info << "Mesh read in = "
    << runTime.cpuTimeIncrement()
    << " s\n" << endl << endl;
  const scalar featureAngle = args.argRead<scalar>(1);
  const bool overwrite = args.optionFound("overwrite");
  const scalar minCos = mousse::cos(degToRad(featureAngle));
  Info << "Feature:" << featureAngle << endl
    << "minCos :" << minCos << endl
    << endl;
  //
  // Use boundaryMesh to reuse all the featureEdge stuff in there.
  //
  boundaryMesh bMesh;
  bMesh.read(mesh);
  // Set feature angle (calculate feature edges)
  bMesh.setFeatureEdges(minCos);
  // Collect all feature edges as edge labels
  labelList markedEdges;
  collectFeatureEdges(bMesh, markedEdges);
  // (new) patch ID for every face in mesh.
  labelList patchIDs{bMesh.mesh().size(), -1};
  //
  // Fill patchIDs with values for every face by floodfilling without
  // crossing feature edge.
  //
  // Current patch number.
  label newPatchI = bMesh.patches().size();
  label suffix = 0;
  while (true) {
    // Find first unset face.
    label unsetFaceI = findIndex(patchIDs, -1);
    if (unsetFaceI == -1) {
      // All faces have patchID set. Exit.
      break;
    }
    // Found unset face. Create patch for it.
    word patchName;
    do {
      patchName = "auto" + name(suffix++);
    }
    while (bMesh.findPatchID(patchName) != -1);
    bMesh.addPatch(patchName);
    bMesh.changePatchType(patchName, "patch");
    // Fill visited with all faces reachable from unsetFaceI.
    boolList visited{bMesh.mesh().size()};
    bMesh.markFaces(markedEdges, unsetFaceI, visited);
    // Assign all visited faces to current patch
    label nVisited = 0;
    FOR_ALL(visited, faceI) {
      if (visited[faceI]) {
        nVisited++;
        patchIDs[faceI] = newPatchI;
      }
    }
    Info << "Assigned " << nVisited << " faces to patch " << patchName
      << endl << endl;
    newPatchI++;
  }
  const PtrList<boundaryPatch>& patches = bMesh.patches();
  // Create new list of patches with old ones first
  List<polyPatch*> newPatchPtrList{patches.size()};
  newPatchI = 0;
  // Copy old patches
  FOR_ALL(mesh.boundaryMesh(), patchI) {
    const polyPatch& patch = mesh.boundaryMesh()[patchI];
    newPatchPtrList[newPatchI] =
      patch.clone
      (
        mesh.boundaryMesh(),
        newPatchI,
        patch.size(),
        patch.start()
      ).ptr();
    newPatchI++;
  }
  // Add new ones with empty size.
  for (label patchI = newPatchI; patchI < patches.size(); patchI++) {
    const boundaryPatch& bp = patches[patchI];
    newPatchPtrList[newPatchI] =
      polyPatch::New
      (
        polyPatch::typeName,
        bp.name(),
        0,
        mesh.nFaces(),
        newPatchI,
        mesh.boundaryMesh()
      ).ptr();
    newPatchI++;
  }
  if (!overwrite) {
    runTime++;
  }
  // Change patches
  repatchPolyTopoChanger polyMeshRepatcher{mesh};
  polyMeshRepatcher.changePatches(newPatchPtrList);
  // Change face ordering
  // Since bMesh read from mesh there is one to one mapping so we don't
  // have to do the geometric stuff.
  const labelList& meshFace = bMesh.meshFace();
  FOR_ALL(patchIDs, faceI) {
    label meshFaceI = meshFace[faceI];
    polyMeshRepatcher.changePatchID(meshFaceI, patchIDs[faceI]);
  }
  polyMeshRepatcher.repatch();
  // Write resulting mesh
  if (overwrite) {
    mesh.setInstance(oldInstance);
  }
  // Set the precision of the points data to 10
  IOstream::defaultPrecision(max(10u, IOstream::defaultPrecision()));
  mesh.write();
  Info << "End\n" << endl;
  return 0;
}

