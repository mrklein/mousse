#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_CENTRED_CPC_CELL_TO_CELL_STENCIL_OBJECT_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_CENTRED_CPC_CELL_TO_CELL_STENCIL_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::centredCPCCellToCellStencilObject
// Description
// SourceFiles
#include "extended_centred_cell_to_cell_stencil.hpp"
#include "cpc_cell_to_cell_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class centredCPCCellToCellStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    centredCPCCellToCellStencilObject
  >,
  public extendedCentredCellToCellStencil
{
public:
  TYPE_NAME("centredCPCCellToCellStencil");
  // Constructors
    //- Construct from uncompacted cell stencil
    explicit centredCPCCellToCellStencilObject
    (
      const fvMesh& mesh
    )
    :
      MeshObject
      <
        fvMesh,
        mousse::TopologicalMeshObject,
        centredCPCCellToCellStencilObject
      >(mesh),
      extendedCentredCellToCellStencil(CPCCellToCellStencil(mesh))
    {}
  //- Destructor
  virtual ~centredCPCCellToCellStencilObject()
  {}
};
}  // namespace mousse
#endif
