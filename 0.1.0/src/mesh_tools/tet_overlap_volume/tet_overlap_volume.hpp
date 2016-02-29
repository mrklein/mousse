#ifndef MESH_TOOLS_TET_OVERLAP_VOLUME_TET_OVERLAP_VOLUME_HPP_
#define MESH_TOOLS_TET_OVERLAP_VOLUME_TET_OVERLAP_VOLUME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetOverlapVolume
// Description
//   Calculates the overlap volume of two cells using tetrahedral decomposition
// SourceFiles
//   tet_overlap_volume.cpp
#include "fixed_list.hpp"
#include "label_list.hpp"
#include "tree_bound_box.hpp"
namespace mousse
{
class primitiveMesh;
class polyMesh;
class tetPoints;
class tetOverlapVolume
{
  // Private member functions
    //- Tet overlap volume
    scalar tetTetOverlapVol
    (
      const tetPoints& tetA,
      const tetPoints& tetB
    ) const;
    //- Return a const treeBoundBox
    treeBoundBox pyrBb
    (
      const pointField& points,
      const face& f,
      const point& fc
    ) const;
public:
  //- Runtime type information
  CLASS_NAME("tetOverlapVolume");
  // Constructors
    //- Null constructor
    tetOverlapVolume();
  // Public members
    //- Return a list of cells in meshA which overlaps with cellBI in
    // meshB
    labelList overlappingCells
    (
      const polyMesh& meshA,
      const polyMesh& meshB,
      const label cellBI
    ) const;
    //- Return true if olverlap volume is greater than threshold
    bool cellCellOverlapMinDecomp
    (
      const primitiveMesh& meshA,
      const label cellAI,
      const primitiveMesh& meshB,
      const label cellBI,
      const treeBoundBox& cellBbB,
      const scalar threshold = 0.0
    ) const;
    //- Calculates the overlap volume
    scalar cellCellOverlapVolumeMinDecomp
    (
      const primitiveMesh& meshA,
      const label cellAI,
      const primitiveMesh& meshB,
      const label cellBI,
      const treeBoundBox& cellBbB
    ) const;
};
}  // namespace mousse
#endif
