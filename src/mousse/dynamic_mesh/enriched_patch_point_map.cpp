// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "enriched_patch.hpp"


// Private Member Functions 
void mousse::enrichedPatch::completePointMap() const
{
  if (pointMapComplete_) {
    FATAL_ERROR_IN("void enrichedPatch::completePointMap() const")
      << "Point map already completed"
      << abort(FatalError);
  }
  pointMapComplete_ = true;
  const Map<label>& pmm = pointMergeMap();
  // Get the mesh points for both patches.  If the point has not been
  // merged away, add it to the map
  // Do master patch
  const labelList& masterMeshPoints = masterPatch_.meshPoints();
  const pointField& masterLocalPoints = masterPatch_.localPoints();
  FOR_ALL(masterMeshPoints, pointI) {
    if (!pmm.found(masterMeshPoints[pointI])) {
      pointMap_.insert(masterMeshPoints[pointI], masterLocalPoints[pointI]);
    }
  }
  // Do slave patch
  const labelList& slaveMeshPoints = slavePatch_.meshPoints();
  const pointField& slaveLocalPoints = slavePatch_.localPoints();
  FOR_ALL(slaveMeshPoints, pointI) {
    if (!pmm.found(slaveMeshPoints[pointI])) {
      pointMap_.insert(slaveMeshPoints[pointI], slaveLocalPoints[pointI]);
    }
  }
}


// Member Functions 
mousse::Map<mousse::point>& mousse::enrichedPatch::pointMap()
{
  if (!pointMapComplete_) {
    completePointMap();
  }
  return pointMap_;
}


const mousse::Map<mousse::point>& mousse::enrichedPatch::pointMap() const
{
  if (!pointMapComplete_) {
    completePointMap();
  }
  return pointMap_;
}

