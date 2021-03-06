#ifndef CORE_MESHES_POLY_MESH_MAP_POLY_MESH_MAP_SUBSET_MESH_HPP_
#define CORE_MESHES_POLY_MESH_MAP_POLY_MESH_MAP_SUBSET_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mapSubsetMesh
// Description
//   Class containing mesh-to-mesh mapping information after a subset operation

#include "ref_count.hpp"
#include "label_list.hpp"


namespace mousse {

class mapSubsetMesh
:
  public refCount
{
  // Private data
    //- Number of old live points
    label nOldPoints_;
    //- Number of old live faces
    label nOldFaces_;
    //- Number of old live cells
    label nOldCells_;
    //- Point mapping array
    const labelList pointMap_;
    //- Face mapping array
    const labelList faceMap_;
    //- Cell mapping array
    const labelList cellMap_;
    //- Patch for exposed faces
    const label exposedPatchID_;
    //- List of the old patch sizes
    labelList oldPatchSizes_;
    //- List of the old patch start labels
    const labelList oldPatchStarts_;
    //- List of numbers of mesh points per old patch
    const labelList oldPatchNMeshPoints_;
public:
  // Constructors
    //- Construct from components
    mapSubsetMesh
    (
      const label nOldPoints,
      const label nOldFaces,
      const label nOldCells,
      const labelList& pointMap,
      const labelList& faceMap,
      const labelList& cellMap,
      const label exposedPatchID,
      const labelList& oldPatchStarts,
      const labelList& oldPatchNMeshPoints
    );
    //- Construct from components and optionally reuse storage
    mapSubsetMesh
    (
      const label nOldPoints,
      const label nOldFaces,
      const label nOldCells,
      labelList& pointMap,
      labelList& faceMap,
      labelList& cellMap,
      const label exposedPatchID,
      labelList& oldPatchStarts,
      labelList& oldPatchNMeshPoints,
      const bool reUse
    );
  // Member Functions
    // Access
      //- Number of old points
      label nOldPoints() const
      {
        return nOldPoints_;
      }
      //- Number of old internal faces
      label nOldInternalFaces() const
      {
        return oldPatchStarts_[0];
      }
      //- Number of old faces
      label nOldFaces() const
      {
        return nOldFaces_;
      }
      //- Number of old cells
      label nOldCells() const
      {
        return nOldCells_;
      }
      //- Patch that exposed faces were put into
      label exposedPatchID() const
      {
        return exposedPatchID_;
      }
      //- Old point map.
      //  Contains the old point label for all points of the subsetted
      //  mesh
      const labelList& pointMap() const
      {
        return pointMap_;
      }
      //- Old face map.
      //  Contains the old point label for all faces of the subsetted
      //  mesh
      const labelList& faceMap() const
      {
        return faceMap_;
      }
      //- Old cell map.
      //  Contains the old point label for all cells of the subsetted
      //  mesh
      const labelList& cellMap() const
      {
        return cellMap_;
      }
      //- Return list of the old patch sizes
      const labelList& oldPatchSizes() const
      {
        return oldPatchSizes_;
      }
      //- Return list of the old patch start labels
      const labelList& oldPatchStarts() const
      {
        return oldPatchStarts_;
      }
      //- Return numbers of mesh points per old patch
      const labelList& oldPatchNMeshPoints() const
      {
        return oldPatchNMeshPoints_;
      }
};
}  // namespace mousse
#endif
