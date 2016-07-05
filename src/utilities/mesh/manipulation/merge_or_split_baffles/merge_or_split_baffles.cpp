// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "sync_tools.hpp"
#include "face_set.hpp"
#include "point_set.hpp"
#include "mesh_tools.hpp"
#include "poly_topo_change.hpp"
#include "poly_remove_face.hpp"
#include "poly_modify_face.hpp"
#include "indirect_primitive_patch.hpp"
#include "processor_poly_patch.hpp"
#include "local_point_region.hpp"
#include "duplicate_points.hpp"
#include "_read_fields.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


using namespace mousse;


void insertDuplicateMerge
(
  const polyMesh& mesh,
  const labelList& duplicates,
  polyTopoChange& meshMod
)
{
  const faceList& faces = mesh.faces();
  const labelList& faceOwner = mesh.faceOwner();
  const faceZoneMesh& faceZones = mesh.faceZones();
  FOR_ALL(duplicates, bFaceI)
  {
    label otherFaceI = duplicates[bFaceI];
    if (otherFaceI == -1 || otherFaceI <= bFaceI)
      continue;
    // Two duplicate faces. Merge.
    label face0 = mesh.nInternalFaces() + bFaceI;
    label face1 = mesh.nInternalFaces() + otherFaceI;
    label own0 = faceOwner[face0];
    label own1 = faceOwner[face1];
    if (own0 < own1) {
      // Use face0 as the new internal face.
      label zoneID = faceZones.whichZone(face0);
      bool zoneFlip = false;
      if (zoneID >= 0) {
        const faceZone& fZone = faceZones[zoneID];
        zoneFlip = fZone.flipMap()[fZone.whichFace(face0)];
      }
      meshMod.setAction(polyRemoveFace(face1));
      meshMod.setAction
      (
        polyModifyFace
        {
          faces[face0],           // modified face
          face0,                  // label of face being modified
          own0,                   // owner
          own1,                   // neighbour
          false,                  // face flip
          -1,                     // patch for face
          false,                  // remove from zone
          zoneID,                 // zone for face
          zoneFlip                // face flip in zone
        }
      );
    } else {
      // Use face1 as the new internal face.
      label zoneID = faceZones.whichZone(face1);
      bool zoneFlip = false;
      if (zoneID >= 0) {
        const faceZone& fZone = faceZones[zoneID];
        zoneFlip = fZone.flipMap()[fZone.whichFace(face1)];
      }
      meshMod.setAction(polyRemoveFace(face0));
      meshMod.setAction
      (
        polyModifyFace
        {
          faces[face1],           // modified face
          face1,                  // label of face being modified
          own1,                   // owner
          own0,                   // neighbour
          false,                  // face flip
          -1,                     // patch for face
          false,                  // remove from zone
          zoneID,                 // zone for face
          zoneFlip                // face flip in zone
        }
      );
    }
  }
}


labelList findBaffles(const polyMesh& mesh, const labelList& boundaryFaces)
{
  // Get all duplicate face labels (in boundaryFaces indices!).
  labelList duplicates =
    localPointRegion::findDuplicateFaces(mesh, boundaryFaces);
  // Check that none are on processor patches
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  FOR_ALL(duplicates, bFaceI) {
    if (duplicates[bFaceI] == -1)
      continue;
    label faceI = mesh.nInternalFaces() + bFaceI;
    label patchI = patches.whichPatch(faceI);
    if (isA<processorPolyPatch>(patches[patchI])) {
      FATAL_ERROR_IN("findBaffles(const polyMesh&, const labelList&)")
        << "Duplicate face " << faceI
        << " is on a processorPolyPatch."
        << "This is not allowed." << nl
        << "Face:" << faceI
        << " is on patch:" << patches[patchI].name()
        << abort(FatalError);
    }
  }

  // Write to faceSet for ease of postprocessing.
  {
    faceSet duplicateSet
    {
      mesh,
      "duplicateFaces",
      (mesh.nFaces() - mesh.nInternalFaces())/256
    };
    FOR_ALL(duplicates, bFaceI) {
      label otherFaceI = duplicates[bFaceI];
      if (otherFaceI != -1 && otherFaceI > bFaceI) {
        duplicateSet.insert(mesh.nInternalFaces() + bFaceI);
        duplicateSet.insert(mesh.nInternalFaces() + otherFaceI);
      }
    }
    Pout << "Writing " << duplicateSet.size()
      << " duplicate faces to faceSet " << duplicateSet.objectPath()
      << nl << endl;
    duplicateSet.write();
  }
  return duplicates;
}


int main(int argc, char *argv[])
{
  argList::addNote
  (
    "Detect faces that share points (baffles).\n"
    "Merge them or duplicate the points."
  );
  #include "add_overwrite_option.inc"
  #include "add_region_option.inc"
  argList::addBoolOption
  (
    "detectOnly",
    "find baffles only, but do not merge or split them"
  );
  argList::addBoolOption
  (
    "split",
    "topologically split duplicate surfaces"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  #include "create_named_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  const bool split = args.optionFound("split");
  const bool overwrite = args.optionFound("overwrite");
  const bool detectOnly = args.optionFound("detectOnly");
  // Collect all boundary faces
  labelList boundaryFaces{mesh.nFaces() - mesh.nInternalFaces()};
  FOR_ALL(boundaryFaces, i) {
    boundaryFaces[i] = i+mesh.nInternalFaces();
  }
  if (detectOnly) {
    findBaffles(mesh, boundaryFaces);
    return 0;
  }
  // Read objects in time directory
  IOobjectList objects{mesh, runTime.timeName()};
  // Read vol fields.
  PtrList<volScalarField> vsFlds;
  ReadFields(mesh, objects, vsFlds);
  PtrList<volVectorField> vvFlds;
  ReadFields(mesh, objects, vvFlds);
  PtrList<volSphericalTensorField> vstFlds;
  ReadFields(mesh, objects, vstFlds);
  PtrList<volSymmTensorField> vsymtFlds;
  ReadFields(mesh, objects, vsymtFlds);
  PtrList<volTensorField> vtFlds;
  ReadFields(mesh, objects, vtFlds);
  // Read surface fields.
  PtrList<surfaceScalarField> ssFlds;
  ReadFields(mesh, objects, ssFlds);
  PtrList<surfaceVectorField> svFlds;
  ReadFields(mesh, objects, svFlds);
  PtrList<surfaceSphericalTensorField> sstFlds;
  ReadFields(mesh, objects, sstFlds);
  PtrList<surfaceSymmTensorField> ssymtFlds;
  ReadFields(mesh, objects, ssymtFlds);
  PtrList<surfaceTensorField> stFlds;
  ReadFields(mesh, objects, stFlds);
  // Mesh change engine
  polyTopoChange meshMod{mesh};
  if (split) {
    Pout << "Topologically splitting duplicate surfaces"
      << ", i.e. duplicating points internal to duplicate surfaces."
      << nl << endl;
    // Analyse which points need to be duplicated
    localPointRegion regionSide{mesh};
    // Point duplication engine
    duplicatePoints pointDuplicator{mesh};
    // Insert topo changes
    pointDuplicator.setRefinement(regionSide, meshMod);
  } else {
    Pout << "Merging duplicate faces." << nl << endl;
    // Get all duplicate face labels (in boundaryFaces indices!).
    labelList duplicates{findBaffles(mesh, boundaryFaces)};
    // Merge into internal faces.
    insertDuplicateMerge(mesh, duplicates, meshMod);
  }
  if (!overwrite) {
    runTime++;
  }
  // Change the mesh. No inflation.
  autoPtr<mapPolyMesh> map = meshMod.changeMesh(mesh, false);
  // Update fields
  mesh.updateMesh(map);
  // Move mesh (since morphing does not do this)
  if (map().hasMotionPoints()) {
    mesh.movePoints(map().preMotionPoints());
  }
  if (overwrite) {
    mesh.setInstance(oldInstance);
  }
  Pout << "Writing mesh to time " << runTime.timeName() << endl;
  mesh.write();
  // Dump duplicated points (if any)
  if (split) {
    const labelList& pointMap = map().pointMap();
    labelList nDupPerPoint{map().nOldPoints(), 0};
    pointSet dupPoints{mesh, "duplicatedPoints", 100};
    FOR_ALL(pointMap, pointI) {
      label oldPointI = pointMap[pointI];
      nDupPerPoint[oldPointI]++;
      if (nDupPerPoint[oldPointI] > 1) {
        dupPoints.insert(map().reversePointMap()[oldPointI]);
        dupPoints.insert(pointI);
      }
    }
    Pout << "Writing " << dupPoints.size()
      << " duplicated points to pointSet "
      << dupPoints.objectPath() << nl << endl;
    dupPoints.write();
  }
  Info << "End\n" << endl;
  return 0;
}

