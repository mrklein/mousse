#ifndef CORE_MESHES_POLY_MESH_MAP_POLY_MESH_MAP_DISTRIBUTE_POLY_MESH_HPP_
#define CORE_MESHES_POLY_MESH_MAP_POLY_MESH_MAP_DISTRIBUTE_POLY_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mapDistributePolyMesh
// Description
//   Class containing mesh-to-mesh mapping information after a mesh distribution
//   where we send parts of meshes (using subsetting) to other processors
//   and receive and reconstruct mesh.
//   We store mapping from the bits-to-send to the complete starting mesh
//   (subXXXMap) and from the received bits to their location in the new
//   mesh (constructXXXMap).
// SourceFiles
//   map_distribute_poly_mesh.cpp


#include "map_distribute.hpp"

namespace mousse
{

class mapPolyMesh;
class polyMesh;

class mapDistributePolyMesh
{

  // Private data

    const polyMesh& mesh_;

    //- Number of old live points
    const label nOldPoints_;

    //- Number of old live faces
    const label nOldFaces_;

    //- Number of old live cells
    const label nOldCells_;

    //- List of the old patch sizes
    labelList oldPatchSizes_;

    //- List of the old patch start labels
    const labelList oldPatchStarts_;

    //- List of numbers of mesh points per old patch
    const labelList oldPatchNMeshPoints_;

    //- Point distribute map
    const mapDistribute pointMap_;

    //- Face distribute map
    const mapDistribute faceMap_;

    //- Cell distribute map
    const mapDistribute cellMap_;

    //- Patch distribute map
    const mapDistribute patchMap_;

  // Private Member Functions
    void calcPatchSizes();

public:

  // Constructors

    //- Construct from components. Note that mesh has to be changed already
    //  since uses mesh.nPoints etc as the new size.
    mapDistributePolyMesh
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
    );

    //- Disallow default bitwise copy construct
    mapDistributePolyMesh(const mapDistributePolyMesh&) = delete;

    //- Disallow default bitwise assignment
    mapDistributePolyMesh& operator=(const mapDistributePolyMesh&) = delete;

  // Member Functions

    // Access

      const polyMesh& mesh() const
      {
        return mesh_;
      }

      //- Number of points in mesh before distribution
      label nOldPoints() const
      {
        return nOldPoints_;
      }

      //- Number of faces in mesh before distribution
      label nOldFaces() const
      {
        return nOldFaces_;
      }

      //- Number of cells in mesh before distribution
      label nOldCells() const
      {
        return nOldCells_;
      }

      //- List of the old patch sizes
      const labelList& oldPatchSizes() const
      {
        return oldPatchSizes_;
      }

      //- List of the old patch start labels
      const labelList& oldPatchStarts() const
      {
        return oldPatchStarts_;
      }

      //- List of numbers of mesh points per old patch
      const labelList& oldPatchNMeshPoints() const
      {
        return oldPatchNMeshPoints_;
      }

      //- Point distribute map
      const mapDistribute& pointMap() const
      {
        return pointMap_;
      }

      //- Face distribute map
      const mapDistribute& faceMap() const
      {
        return faceMap_;
      }

      //- Cell distribute map
      const mapDistribute& cellMap() const
      {
        return cellMap_;
      }

      //- Patch distribute map
      const mapDistribute& patchMap() const
      {
        return patchMap_;
      }

    // Edit

      //- Distribute list of point data
      template<class T>
      void distributePointData(List<T>& lst) const
      {
        pointMap_.distribute(lst);
      }

      //- Distribute list of face data
      template<class T>
      void distributeFaceData(List<T>& lst) const
      {
        faceMap_.distribute(lst);
      }

      //- Distribute list of cell data
      template<class T>
      void distributeCellData(List<T>& lst) const
      {
        cellMap_.distribute(lst);
      }

      //- Distribute list of patch data
      template<class T>
      void distributePatchData(List<T>& lst) const
      {
        patchMap_.distribute(lst);
      }

      //- Distribute list of point/face/cell/patch indices.
      //  (Converts to boolList, distributes boolList and reconstructs)
      void distributePointIndices(labelList& pointIDs) const;
      void distributeFaceIndices(labelList& faceIDs) const;
      void distributeCellIndices(labelList& cellIDs) const;
      void distributePatchIndices(labelList& patchIDs) const;

      //- Correct for topo change.
      void updateMesh(const mapPolyMesh&)
      {
        NOT_IMPLEMENTED
        (
          "mapDistributePolyMesh::updateMesh(const mapPolyMesh&)"
        );
      }

};

}  // namespace mousse
#endif
