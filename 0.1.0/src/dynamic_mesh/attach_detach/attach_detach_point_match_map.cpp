// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "attach_detach.hpp"
#include "poly_mesh.hpp"
#include "primitive_mesh.hpp"
#include "primitive_face_patch.hpp"
#include "poly_topo_changer.hpp"
// Private Member Functions 
const mousse::Map<mousse::label>&
mousse::attachDetach::pointMatchMap() const
{
  if (!pointMatchMapPtr_)
  {
    calcPointMatchMap();
  }
  return *pointMatchMapPtr_;
}
void mousse::attachDetach::calcPointMatchMap() const
{
  if (debug)
  {
    Pout<< "void attachDetach::calcPointMatchMap() const "
      << " for object " << name() << " : "
      << "Calculating point matching" << endl;
  }
  if (pointMatchMapPtr_)
  {
    FatalErrorIn
    (
      "void attachDetach::calcPointMatchMap() const"
    )   << "Point match map already calculated for object " << name()
      << abort(FatalError);
  }
  const polyMesh& mesh = topoChanger().mesh();
  const faceList& faces = mesh.faces();
  const polyPatch& masterPatch = mesh.boundaryMesh()[masterPatchID_.index()];
  const polyPatch& slavePatch = mesh.boundaryMesh()[slavePatchID_.index()];
  // Create the reverse patch out of the slave patch
  primitiveFacePatch reverseSlavePatch
  (
    faceList(slavePatch.size()),
    mesh.points()
  );
  const label slavePatchStart = slavePatch.start();
  forAll(reverseSlavePatch, faceI)
  {
    reverseSlavePatch[faceI] =
      faces[slavePatchStart + faceI].reverseFace();
  }
  // Create point merge list and remove merged points
  const labelList& masterMeshPoints = masterPatch.meshPoints();
  const labelList& slaveMeshPoints = reverseSlavePatch.meshPoints();
  const faceList& masterLocalFaces = masterPatch.localFaces();
  const faceList& slaveLocalFaces = reverseSlavePatch.localFaces();
  pointMatchMapPtr_ = new Map<label>(2*slaveMeshPoints.size());
  Map<label>& removedPointMap = *pointMatchMapPtr_;
  forAll(masterLocalFaces, faceI)
  {
    const face& curMasterPoints = masterLocalFaces[faceI];
    const face& curSlavePoints = slaveLocalFaces[faceI];
    forAll(curMasterPoints, pointI)
    {
      // If the master and slave point labels are the same, the
      // point remains.  Otherwise, the slave point is removed and
      // replaced by the master
      if
      (
        masterMeshPoints[curMasterPoints[pointI]]
      != slaveMeshPoints[curSlavePoints[pointI]]
      )
      {
        // Pout<< "Matching slave point "
        //     << slaveMeshPoints[curSlavePoints[pointI]]
        //     << " with "
        //     << masterMeshPoints[curMasterPoints[pointI]]
        //     << endl;
        // Grab the addressing
        removedPointMap.insert
        (
          slaveMeshPoints[curSlavePoints[pointI]],
          masterMeshPoints[curMasterPoints[pointI]]
        );
      }
    }
  }
  if (debug)
  {
    Pout<< "void attachDetach::calcPointMatchMap() const "
      << " for object " << name() << " : "
      << "Finished calculating point matching" << endl;
  }
}
