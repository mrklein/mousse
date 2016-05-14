// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pstream_reduce_ops.hpp"
#include "arg_list.hpp"
#include "time.hpp"
#include "poly_topo_change.hpp"
#include "poly_modify_face.hpp"
#include "poly_add_face.hpp"
#include "combine_faces.hpp"
#include "remove_points.hpp"
#include "poly_mesh.hpp"
#include "map_poly_mesh.hpp"
#include "unit_conversion.hpp"
#include "motion_smoother.hpp"


using namespace mousse;


// Merge faces on the same patch (usually from exposing refinement)
// Can undo merges if these cause problems.
label mergePatchFaces
(
  const scalar minCos,
  const scalar concaveSin,
  const autoPtr<IOdictionary>& qualDictPtr,
  const Time& /*runTime*/,
  polyMesh& mesh
)
{
  // Patch face merging engine
  combineFaces faceCombiner{mesh};
  // Get all sets of faces that can be merged
  labelListList allFaceSets{faceCombiner.getMergeSets(minCos, concaveSin)};
  label nFaceSets = returnReduce(allFaceSets.size(), sumOp<label>());
  Info << "Merging " << nFaceSets << " sets of faces." << endl;
  if (nFaceSets > 0) {
    // Store the faces of the face sets
    List<faceList> allFaceSetsFaces{allFaceSets.size()};
    FOR_ALL(allFaceSets, setI) {
      allFaceSetsFaces[setI] =
        UIndirectList<face>
        {
          mesh.faces(),
          allFaceSets[setI]
        };
    }

    autoPtr<mapPolyMesh> map;

    {
      // Topology changes container
      polyTopoChange meshMod{mesh};
      // Merge all faces of a set into the first face of the set.
      faceCombiner.setRefinement(allFaceSets, meshMod);
      // Change the mesh (no inflation)
      map = meshMod.changeMesh(mesh, false, true);
      // Update fields
      mesh.updateMesh(map);
      // Move mesh (since morphing does not do this)
      if (map().hasMotionPoints()) {
        mesh.movePoints(map().preMotionPoints());
      } else {
        // Delete mesh volumes. No other way to do this?
        mesh.clearOut();
      }
    }
    // Check for errors and undo
    // ~~~~~~~~~~~~~~~~~~~~~~~~~
    // Faces in error.
    labelHashSet errorFaces;
    if (qualDictPtr.valid()) {
      motionSmoother::checkMesh(false, mesh, qualDictPtr(), errorFaces);
    } else {
      mesh.checkFacePyramids(false, -SMALL, &errorFaces);
    }
    // Sets where the master is in error
    labelHashSet errorSets;
    FOR_ALL(allFaceSets, setI) {
      label newMasterI = map().reverseFaceMap()[allFaceSets[setI][0]];
      if (errorFaces.found(newMasterI)) {
        errorSets.insert(setI);
      }
    }
    label nErrorSets = returnReduce(errorSets.size(), sumOp<label>());
    Info << "Detected " << nErrorSets
      << " error faces on boundaries that have been merged."
      << " These will be restored to their original faces."
      << endl;
    if (nErrorSets > 0) {
      // Renumber stored faces to new vertex numbering.
      FOR_ALL_CONST_ITER(labelHashSet, errorSets, iter) {
        label setI = iter.key();
        faceList& setFaceVerts = allFaceSetsFaces[setI];
        FOR_ALL(setFaceVerts, i)
        {
          inplaceRenumber(map().reversePointMap(), setFaceVerts[i]);
          // Debug: check that all points are still there.
          FOR_ALL(setFaceVerts[i], j) {
            label newVertI = setFaceVerts[i][j];
            if (newVertI < 0) {
              FATAL_ERROR_IN("mergePatchFaces")
                << "In set:" << setI << " old face labels:"
                << allFaceSets[setI] << " new face vertices:"
                << setFaceVerts[i] << " are unmapped vertices!"
                << abort(FatalError);
            }
          }
        }
      }
      // Topology changes container
      polyTopoChange meshMod{mesh};
      // Restore faces
      FOR_ALL_CONST_ITER(labelHashSet, errorSets, iter) {
        label setI = iter.key();
        const labelList& setFaces = allFaceSets[setI];
        const faceList& setFaceVerts = allFaceSetsFaces[setI];
        label newMasterI = map().reverseFaceMap()[setFaces[0]];
        // Restore. Get face properties.
        label own = mesh.faceOwner()[newMasterI];
        label zoneID = mesh.faceZones().whichZone(newMasterI);
        bool zoneFlip = false;
        if (zoneID >= 0) {
          const faceZone& fZone = mesh.faceZones()[zoneID];
          zoneFlip = fZone.flipMap()[fZone.whichFace(newMasterI)];
        }
        label patchID = mesh.boundaryMesh().whichPatch(newMasterI);
        Pout << "Restoring new master face " << newMasterI
          << " to vertices " << setFaceVerts[0] << endl;
        // Modify the master face.
        meshMod.setAction
          (
            polyModifyFace
            {
              setFaceVerts[0],                // original face
              newMasterI,                     // label of face
              own,                            // owner
              -1,                             // neighbour
              false,                          // face flip
              patchID,                        // patch for face
              false,                          // remove from zone
              zoneID,                         // zone for face
              zoneFlip                        // face flip in zone
            }
          );
        // Add the previously removed faces
        for (label i = 1; i < setFaces.size(); i++) {
          Pout << "Restoring removed face " << setFaces[i]
            << " with vertices " << setFaceVerts[i] << endl;
          meshMod.setAction
            (
              polyAddFace
              {
                setFaceVerts[i],        // vertices
                own,                    // owner,
                -1,                     // neighbour,
                -1,                     // masterPointID,
                -1,                     // masterEdgeID,
                newMasterI,             // masterFaceID,
                false,                  // flipFaceFlux,
                patchID,                // patchID,
                zoneID,                 // zoneID,
                zoneFlip                // zoneFlip
              }
            );
        }
      }
      // Change the mesh (no inflation)
      map = meshMod.changeMesh(mesh, false, true);
      // Update fields
      mesh.updateMesh(map);
      // Move mesh (since morphing does not do this)
      if (map().hasMotionPoints()) {
        mesh.movePoints(map().preMotionPoints());
      } else {
        // Delete mesh volumes. No other way to do this?
        mesh.clearOut();
      }
    }
  } else {
    Info << "No faces merged ..." << endl;
  }
  return nFaceSets;
}


// Remove points not used by any face or points used by only two faces where
// the edges are in line
label mergeEdges(const scalar minCos, polyMesh& mesh)
{
  Info << "Merging all points on surface that" << nl
    << "- are used by only two boundary faces and" << nl
    << "- make an angle with a cosine of more than " << minCos
    << "." << nl << endl;
  // Point removal analysis engine
  removePoints pointRemover{mesh};
  // Count usage of points
  boolList pointCanBeDeleted;
  label nRemove = pointRemover.countPointUsage(minCos, pointCanBeDeleted);
  if (nRemove > 0) {
    Info << "Removing " << nRemove
      << " straight edge points ..." << endl;
    // Topology changes container
    polyTopoChange meshMod{mesh};
    pointRemover.setRefinement(pointCanBeDeleted, meshMod);
    // Change the mesh (no inflation)
    autoPtr<mapPolyMesh> map = meshMod.changeMesh(mesh, false, true);
    // Update fields
    mesh.updateMesh(map);
    // Move mesh (since morphing does not do this)
    if (map().hasMotionPoints()) {
      mesh.movePoints(map().preMotionPoints());
    } else {
      // Delete mesh volumes. No other way to do this?
      mesh.clearOut();
    }
  } else {
    Info << "No straight edges simplified and no points removed ..." << endl;
  }
  return nRemove;
}


int main(int argc, char *argv[])
{
  #include "add_overwrite_option.inc"
  argList::validArgs.append("featureAngle [0..180]");
  argList::addOption
  (
    "concaveAngle",
    "degrees",
    "specify concave angle [0..180] (default: 30 degrees)"
  );
  argList::addBoolOption
  (
    "meshQuality",
    "read user-defined mesh quality criterions from system/meshQualityDict"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  #include "create_poly_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  const scalar featureAngle = args.argRead<scalar>(1);
  const scalar minCos = mousse::cos(degToRad(featureAngle));
  // Sin of angle between two consecutive edges on a face.
  // If sin(angle) larger than this the face will be considered concave.
  scalar concaveAngle = args.optionLookupOrDefault("concaveAngle", 30.0);
  scalar concaveSin = mousse::sin(degToRad(concaveAngle));
  const bool overwrite = args.optionFound("overwrite");
  const bool meshQuality = args.optionFound("meshQuality");
  Info << "Merging all faces of a cell" << nl
    << "    - which are on the same patch" << nl
    << "    - which make an angle < " << featureAngle << " degrees"
    << nl
    << "      (cos:" << minCos << ')' << nl
    << "    - even when resulting face becomes concave by more than "
    << concaveAngle << " degrees" << nl
    << "      (sin:" << concaveSin << ')' << nl
    << endl;
  autoPtr<IOdictionary> qualDict;
  if (meshQuality) {
    Info << "Enabling user-defined geometry checks." << nl << endl;
    qualDict.reset
    (
      new IOdictionary
      {
        {
          "meshQualityDict",
          mesh.time().system(),
          mesh,
          IOobject::MUST_READ
        }
      }
    );
  }
  if (!overwrite) {
    runTime++;
  }
  // Merge faces on same patch
  label nChanged =
    mergePatchFaces
    (
      minCos,
      concaveSin,
      qualDict,
      runTime,
      mesh
    );
  // Merge points on straight edges and remove unused points
  if (qualDict.valid()) {
    Info << "Merging all 'loose' points on surface edges, "
      << "regardless of the angle they make." << endl;
    // Surface bnound to be used to extrude. Merge all loose points.
    nChanged += mergeEdges(-1, mesh);
  } else {
    nChanged += mergeEdges(minCos, mesh);
  }
  if (nChanged > 0) {
    if (overwrite) {
      mesh.setInstance(oldInstance);
    }
    Info << "Writing morphed mesh to time " << runTime.timeName() << endl;
    mesh.write();
  } else {
    Info << "Mesh unchanged." << endl;
  }
  Info << "\nEnd\n" << endl;
  return 0;
}

