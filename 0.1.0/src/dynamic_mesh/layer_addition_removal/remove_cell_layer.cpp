// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "layer_addition_removal.hpp"
#include "poly_mesh.hpp"
#include "primitive_mesh.hpp"
#include "poly_topo_change.hpp"
#include "opposite_face.hpp"
#include "poly_topo_changer.hpp"
#include "poly_remove_cell.hpp"
#include "poly_remove_face.hpp"
#include "poly_remove_point.hpp"
#include "poly_modify_face.hpp"
// Private Member Functions 
bool mousse::layerAdditionRemoval::validCollapse() const
{
  // Check for valid layer collapse
  // - no boundary-to-boundary collapse
  if (debug)
  {
    Pout<< "Checking layer collapse for object " << name() << endl;
  }
  // Grab the face collapse mapping
  const polyMesh& mesh = topoChanger().mesh();
  const labelList& ftc = facesPairing();
  const labelList& mf = mesh.faceZones()[faceZoneID_.index()];
  label nBoundaryHits = 0;
  FOR_ALL(mf, faceI)
  {
    if
    (
      !mesh.isInternalFace(mf[faceI])
    && !mesh.isInternalFace(ftc[faceI])
    )
    {
      nBoundaryHits++;
    }
  }
  if (debug)
  {
    Pout<< "Finished checking layer collapse for object "
      << name() <<".  Number of boundary-on-boundary hits: "
      << nBoundaryHits << endl;
  }
  if (returnReduce(nBoundaryHits, sumOp<label>()) > 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}
void mousse::layerAdditionRemoval::removeCellLayer
(
  polyTopoChange& ref
) const
{
  // Algorithm for layer removal.  Second phase: topological change
  // 2)  Go through all the faces of the master cell layer and remove
  //     the ones that are not in the master face zone.
  //
  // 3)  Grab all the faces coming out of points that are collapsed
  //     and select the ones that are not marked for removal.  Go
  //     through the remaining faces and replace the point to remove by
  //     the equivalent point in the master face zone.
  if (debug)
  {
    Pout<< "Removing the cell layer for object " << name() << endl;
  }
  const polyMesh& mesh = topoChanger().mesh();
  const labelList& own = mesh.faceOwner();
  const labelList& nei = mesh.faceNeighbour();
  const labelList& ptc = pointsPairing();
  const labelList& ftc = facesPairing();
  // Remove all the cells from the master layer
  const labelList& mc =
    mesh.faceZones()[faceZoneID_.index()].masterCells();
  FOR_ALL(mc, faceI)
  {
    label slaveSideCell = own[ftc[faceI]];
    if (mesh.isInternalFace(ftc[faceI]) && slaveSideCell == mc[faceI])
    {
      // Owner cell of the face is being removed.
      // Grab the neighbour instead
      slaveSideCell = nei[ftc[faceI]];
    }
    ref.setAction(polyRemoveCell(mc[faceI], slaveSideCell));
  }
  // Remove all the faces from the master layer cells which are not in
  // the master face layer
  labelHashSet facesToRemoveMap(mc.size()*primitiveMesh::facesPerCell_);
  const cellList& cells = mesh.cells();
  FOR_ALL(mc, cellI)
  {
    const cell& curCell = cells[mc[cellI]];
    FOR_ALL(curCell, faceI)
    {
      // Check if the face is in the master zone.  If not, remove it
      if
      (
        mesh.faceZones().whichZone(curCell[faceI])
      != faceZoneID_.index()
      )
      {
        facesToRemoveMap.insert(curCell[faceI]);
      }
    }
  }
  FOR_ALL_CONST_ITER(labelHashSet, facesToRemoveMap, iter)
  {
    ref.setAction(polyRemoveFace(iter.key()));
  }
  // Remove all points that will be collapsed
  FOR_ALL(ptc, pointI)
  {
    ref.setAction(polyRemovePoint(ptc[pointI]));
  }
  // Grab all faces coming off points to be deleted.  If the face
  // has not been deleted, replace the removed point with the
  // equivalent from the master layer.
  // Make a map of all point to be removed, giving the master point label
  // for each of them
  Map<label> removedPointMap(2*ptc.size());
  const labelList& meshPoints =
    mesh.faceZones()[faceZoneID_.index()]().meshPoints();
  FOR_ALL(ptc, pointI)
  {
    removedPointMap.insert(ptc[pointI], meshPoints[pointI]);
  }
  const labelListList& pf = mesh.pointFaces();
  const faceList& faces = mesh.faces();
  // Make a list of faces to be modified using the map to avoid duplicates
  labelHashSet facesToModify(ptc.size()*primitiveMesh::facesPerPoint_);
  FOR_ALL(ptc, pointI)
  {
    const labelList& curFaces = pf[ptc[pointI]];
    FOR_ALL(curFaces, faceI)
    {
      if (!facesToRemoveMap.found(curFaces[faceI]))
      {
        facesToModify.insert(curFaces[faceI]);
      }
    }
  }
  labelList ftm = facesToModify.toc();
  if (debug > 1)
  {
    Pout<< "faces to modify: " << ftm << endl;
  }
  FOR_ALL(ftm, faceI)
  {
    // For every face to modify, copy the face and re-map the vertices.
    // It is known all the faces will be changed since they hang off
    // re-mapped vertices
    label curFaceID = ftm[faceI];
    face newFace(faces[curFaceID]);
    FOR_ALL(newFace, pointI)
    {
      Map<label>::iterator rpmIter =
        removedPointMap.find(newFace[pointI]);
      if (rpmIter != removedPointMap.end())
      {
        // Point mapped. Replace it
        newFace[pointI] = rpmIter();
      }
    }
    if (debug > 1)
    {
      Pout<< "face label: " << curFaceID
        << " old face: " << faces[curFaceID]
        << " new face: " << newFace << endl;
    }
    // Get face zone and its flip
    label modifiedFaceZone = mesh.faceZones().whichZone(curFaceID);
    bool modifiedFaceZoneFlip = false;
    if (modifiedFaceZone >= 0)
    {
      const faceZone& fz = mesh.faceZones()[modifiedFaceZone];
      modifiedFaceZoneFlip = fz.flipMap()[fz.whichFace(curFaceID)];
    }
    label newNeighbour = -1;
    if (curFaceID < mesh.nInternalFaces())
    {
      newNeighbour = nei[curFaceID];
    }
    // Modify the face
    ref.setAction
    (
      polyModifyFace
      (
        newFace,                // modified face
        curFaceID,              // label of face being modified
        own[curFaceID],         // owner
        newNeighbour,           // neighbour
        false,                  // face flip
        mesh.boundaryMesh().whichPatch(curFaceID),// patch for face
        false,                  // remove from zone
        modifiedFaceZone,       // zone for face
        modifiedFaceZoneFlip    // face flip in zone
      )
    );
  }
  // Modify the faces in the master layer to point past the removed cells
  const labelList& mf = mesh.faceZones()[faceZoneID_.index()];
  const boolList& mfFlip = mesh.faceZones()[faceZoneID_.index()].flipMap();
  FOR_ALL(mf, faceI)
  {
    // Grab the owner and neighbour of the faces to be collapsed and get rid
    // of the cell to be removed
    label masterSideCell = own[mf[faceI]];
    if (masterSideCell == mc[faceI])
    {
      if (mesh.isInternalFace(mf[faceI]))
      {
        // Owner cell of the face is being removed.
        // Grab the neighbour instead
        masterSideCell = nei[mf[faceI]];
      }
      else
      {
        masterSideCell = -1;
      }
    }
    label slaveSideCell = own[ftc[faceI]];
    if (slaveSideCell == mc[faceI])
    {
      if (mesh.isInternalFace(ftc[faceI]))
      {
        // Owner cell of the face is being removed.
        // Grab the neighbour instead
        slaveSideCell = nei[ftc[faceI]];
      }
      else
      {
        slaveSideCell = -1;
      }
    }
    // Find out if the face needs to be flipped
    label newOwner = -1;
    label newNeighbour = -1;
    bool flipFace = false;
    label newPatchID = -1;
    label newZoneID = -1;
    // Is any of the faces a boundary face?  If so, grab the patch
    // A boundary-to-boundary collapse is checked for in validCollapse()
    // and cannot happen here.
    if (!mesh.isInternalFace(mf[faceI]))
    {
      // Master is the boundary face: it gets a new owner but no flip
      newOwner = slaveSideCell;
      newNeighbour = -1;
      flipFace = false;
      newPatchID = mesh.boundaryMesh().whichPatch(mf[faceI]);
      newZoneID = mesh.faceZones().whichZone(mf[faceI]);
    }
    else if (!mesh.isInternalFace(ftc[faceI]))
    {
      // Slave is the boundary face: grab its patch
      newOwner = slaveSideCell;
      newNeighbour = -1;
      // Find out if the face flip is necessary
      if (own[mf[faceI]] == slaveSideCell)
      {
        flipFace = false;
      }
      else
      {
        flipFace = true;
      }
      newPatchID = mesh.boundaryMesh().whichPatch(ftc[faceI]);
      // The zone of the master face is preserved
      newZoneID = mesh.faceZones().whichZone(mf[faceI]);
    }
    else
    {
      // Both faces are internal: flip is decided based on which of the
      // new cells around it has a lower label
      newOwner = min(masterSideCell, slaveSideCell);
      newNeighbour = max(masterSideCell, slaveSideCell);
      if (newOwner == own[mf[faceI]] || newNeighbour == nei[mf[faceI]])
      {
        flipFace = false;
      }
      else
      {
        flipFace = true;
      }
      newPatchID = -1;
      // The zone of the master face is preserved
      newZoneID = mesh.faceZones().whichZone(mf[faceI]);
    }
    // Modify the face and flip if necessary
    face newFace = faces[mf[faceI]];
    bool zoneFlip = mfFlip[faceI];
    if (flipFace)
    {
      newFace.flip();
      zoneFlip = !zoneFlip;
    }
    if (debug > 1)
    {
      Pout<< "Modifying face " << mf[faceI]
        << " newFace: " << newFace << nl
        << " newOwner: " << newOwner
        << " newNeighbour: " << newNeighbour
        << " flipFace: " << flipFace
        << " newPatchID: " << newPatchID
        << " newZoneID: " << newZoneID << nl
        << " oldOwn: " << own[mf[faceI]]
        << " oldNei: " << nei[mf[faceI]] << endl;
    }
    ref.setAction
    (
      polyModifyFace
      (
        newFace,       // modified face
        mf[faceI],     // label of face being modified
        newOwner,      // owner
        newNeighbour,  // neighbour
        flipFace,      // flip
        newPatchID,    // patch for face
        false,         // remove from zone
        newZoneID,     // new zone
        zoneFlip       // face flip in zone
      )
    );
  }
}
