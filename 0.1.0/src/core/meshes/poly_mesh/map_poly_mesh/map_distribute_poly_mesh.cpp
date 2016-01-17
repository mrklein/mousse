// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "map_distribute_poly_mesh.hpp"
#include "poly_mesh.hpp"
// Private Member Functions 
void mousse::mapDistributePolyMesh::calcPatchSizes()
{
  oldPatchSizes_.setSize(oldPatchStarts_.size());
  // Calculate old patch sizes
  for (label patchI = 0; patchI < oldPatchStarts_.size() - 1; patchI++)
  {
    oldPatchSizes_[patchI] =
      oldPatchStarts_[patchI + 1] - oldPatchStarts_[patchI];
  }
  // Set the last one by hand
  const label lastPatchID = oldPatchStarts_.size() - 1;
  oldPatchSizes_[lastPatchID] = nOldFaces_ - oldPatchStarts_[lastPatchID];
  if (min(oldPatchSizes_) < 0)
  {
    FATAL_ERROR_IN("mapDistributePolyMesh::calcPatchSizes()")
      << "Calculated negative old patch size:" << oldPatchSizes_ << nl
      << "Error in mapping data" << abort(FatalError);
  }
}
// Constructors 
//- Construct from components
mousse::mapDistributePolyMesh::mapDistributePolyMesh
(
  const polyMesh& mesh,
  // mesh before changes
  const label nOldPoints,
  const label nOldFaces,
  const label nOldCells,
  const Xfer<labelList>& oldPatchStarts,
  const Xfer<labelList>& oldPatchNMeshPoints,
  // how to subset pieces of mesh to send across
  const Xfer<labelListList>& subPointMap,
  const Xfer<labelListList>& subFaceMap,
  const Xfer<labelListList>& subCellMap,
  const Xfer<labelListList>& subPatchMap,
  // how to reconstruct received mesh
  const Xfer<labelListList>& constructPointMap,
  const Xfer<labelListList>& constructFaceMap,
  const Xfer<labelListList>& constructCellMap,
  const Xfer<labelListList>& constructPatchMap
)
:
  mesh_(mesh),
  nOldPoints_(nOldPoints),
  nOldFaces_(nOldFaces),
  nOldCells_(nOldCells),
  oldPatchSizes_(oldPatchStarts().size()),
  oldPatchStarts_(oldPatchStarts),
  oldPatchNMeshPoints_(oldPatchNMeshPoints),
  pointMap_(mesh.nPoints(), subPointMap, constructPointMap),
  faceMap_(mesh.nFaces(), subFaceMap, constructFaceMap),
  cellMap_(mesh.nCells(), subCellMap, constructCellMap),
  patchMap_(mesh.boundaryMesh().size(), subPatchMap, constructPatchMap)
{
  calcPatchSizes();
}
// Member Functions 
void mousse::mapDistributePolyMesh::distributePointIndices(labelList& lst) const
{
  // Construct boolList from selected elements
  boolList isSelected
  (
    createWithValues<boolList>
    (
      nOldPoints(),
      false,
      lst,
      true
    )
  );
  // Distribute
  distributePointData(isSelected);
  // Collect selected elements
  lst = findIndices(isSelected, true);
}
void mousse::mapDistributePolyMesh::distributeFaceIndices(labelList& lst) const
{
  // Construct boolList from selected elements
  boolList isSelected
  (
    createWithValues<boolList>
    (
      nOldFaces(),
      false,
      lst,
      true
    )
  );
  // Distribute
  distributeFaceData(isSelected);
  // Collect selected elements
  lst = findIndices(isSelected, true);
}
void mousse::mapDistributePolyMesh::distributeCellIndices(labelList& lst) const
{
  // Construct boolList from selected elements
  boolList isSelected
  (
    createWithValues<boolList>
    (
      nOldCells(),
      false,
      lst,
      true
    )
  );
  // Distribute
  distributeCellData(isSelected);
  // Collect selected elements
  lst = findIndices(isSelected, true);
}
void mousse::mapDistributePolyMesh::distributePatchIndices(labelList& lst) const
{
  // Construct boolList from selected elements
  boolList isSelected
  (
    createWithValues<boolList>
    (
      oldPatchStarts().size(),    // nOldPatches
      false,
      lst,
      true
    )
  );
  // Distribute
  distributePatchData(isSelected);
  // Collect selected elements
  lst = findIndices(isSelected, true);
}
// Friend Functions 
// Friend Operators
