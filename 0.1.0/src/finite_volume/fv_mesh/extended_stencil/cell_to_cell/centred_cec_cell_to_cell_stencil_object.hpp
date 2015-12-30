// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::centredCECCellToCellStencilObject
// Description
// SourceFiles
#ifndef centred_cec_cell_to_cell_stencil_object_hpp_
#define centred_cec_cell_to_cell_stencil_object_hpp_
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
  TypeName("centredCECCellToCellStencil");
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
