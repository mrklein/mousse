#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_CENTRED_CEC_CELL_TO_FACE_STENCIL_OBJECT_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_CENTRED_CEC_CELL_TO_FACE_STENCIL_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::centredCECCellToFaceStencilObject
// Description
// SourceFiles
#include "extended_centred_cell_to_face_stencil.hpp"
#include "cec_cell_to_face_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class centredCECCellToFaceStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    centredCECCellToFaceStencilObject
  >,
  public extendedCentredCellToFaceStencil
{
public:
  TYPE_NAME("centredCECCellToFaceStencil");
  // Constructors
    //- Construct from uncompacted face stencil
    explicit centredCECCellToFaceStencilObject
    (
      const fvMesh& mesh
    )
    :
      MeshObject
      <
        fvMesh,
        mousse::TopologicalMeshObject,
        centredCECCellToFaceStencilObject
      >(mesh),
      extendedCentredCellToFaceStencil(CECCellToFaceStencil(mesh))
    {
      if (extendedCellToFaceStencil::debug)
      {
        Info<< "Generated centred stencil " << type()
          << nl << endl;
        writeStencilStats(Info, stencil(), map());
      }
    }
  //- Destructor
  virtual ~centredCECCellToFaceStencilObject()
  {}
};
}  // namespace mousse
#endif
