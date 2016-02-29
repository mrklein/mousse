#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_CENTRED_CEC_CELL_TO_CELL_STENCIL_OBJECT_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_CENTRED_CEC_CELL_TO_CELL_STENCIL_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::centredCECCellToCellStencilObject
// Description
// SourceFiles
#include "extended_centred_cell_to_cell_stencil.hpp"
#include "cec_cell_to_cell_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class centredCECCellToCellStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    centredCECCellToCellStencilObject
  >,
  public extendedCentredCellToCellStencil
{
public:
  TYPE_NAME("centredCECCellToCellStencil");
  // Constructors
    //- Construct from uncompacted cell stencil
    explicit centredCECCellToCellStencilObject
    (
      const fvMesh& mesh
    )
    :
      MeshObject
      <
        fvMesh,
        mousse::TopologicalMeshObject,
        centredCECCellToCellStencilObject
      >(mesh),
      extendedCentredCellToCellStencil(CECCellToCellStencil(mesh))
    {}
  //- Destructor
  virtual ~centredCECCellToCellStencilObject()
  {}
};
}  // namespace mousse
#endif
