#ifndef DYNAMIC_MESH_MESH_CUT_WALL_LAYER_CELLS_WALL_LAYER_CELLS_HPP_
#define DYNAMIC_MESH_MESH_CUT_WALL_LAYER_CELLS_WALL_LAYER_CELLS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallLayerCells
// Description
//   After construction is a list of \<refineCell\> of cells within a certain
//   distance to the wall and with the wall normal direction.
// SourceFiles
//   wall_layer_cells.cpp
#include "edge_vertex.hpp"
#include "refine_cell.hpp"
#include "type_info.hpp"
namespace mousse
{
class wallLayerCells
:
  public edgeVertex,
  public List<refineCell>
{
  // Private Member Functions
    //- Check if any of the faces of cellI is on processorPatch
    bool usesCoupledPatch(const label cellI) const;
public:
  //- Runtime type information
  CLASS_NAME("wallLayerCells");
  // Constructors
    //- Construct from components
    wallLayerCells
    (
      const polyMesh& mesh,
      const List<word>& patchNames,
      const label nLayers
    );
};
}  // namespace mousse
#endif
