// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::centredCFCFaceToCellStencilObject
// Description
// SourceFiles
#ifndef centred_cfc_face_to_cell_stencil_object_hpp_
#define centred_cfc_face_to_cell_stencil_object_hpp_
#include "extended_centred_face_to_cell_stencil.hpp"
#include "cfc_face_to_cell_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class centredCFCFaceToCellStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    centredCFCFaceToCellStencilObject
  >,
  public extendedCentredFaceToCellStencil
{
public:
  TypeName("centredCFCFaceToCellStencil");
  // Constructors
    //- Construct from uncompacted face stencil
    explicit centredCFCFaceToCellStencilObject
    (
      const fvMesh& mesh
    )
    :
      MeshObject
      <
        fvMesh,
        mousse::TopologicalMeshObject,
        centredCFCFaceToCellStencilObject
      >(mesh),
      extendedCentredFaceToCellStencil(CFCFaceToCellStencil(mesh))
    {}
  //- Destructor
  virtual ~centredCFCFaceToCellStencilObject()
  {}
};
}  // namespace mousse
#endif
