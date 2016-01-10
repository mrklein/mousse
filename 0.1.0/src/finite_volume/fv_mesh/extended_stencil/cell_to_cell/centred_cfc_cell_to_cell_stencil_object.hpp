// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::centredCFCCellToCellStencilObject
// Description
// SourceFiles
#ifndef centred_cfc_cell_to_cell_stencil_object_hpp_
#define centred_cfc_cell_to_cell_stencil_object_hpp_
#include "extended_centred_cell_to_cell_stencil.hpp"
#include "cfc_cell_to_cell_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class centredCFCCellToCellStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    centredCFCCellToCellStencilObject
  >,
  public extendedCentredCellToCellStencil
{
public:
  TYPE_NAME("centredCFCCellToCellStencil");
  // Constructors
    //- Construct from uncompacted cell stencil
    explicit centredCFCCellToCellStencilObject
    (
      const fvMesh& mesh
    )
    :
      MeshObject
      <
        fvMesh,
        mousse::TopologicalMeshObject,
        centredCFCCellToCellStencilObject
      >(mesh),
      extendedCentredCellToCellStencil(CFCCellToCellStencil(mesh))
    {}
  //- Destructor
  virtual ~centredCFCCellToCellStencilObject()
  {}
};
}  // namespace mousse
#endif
