#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_CENTRED_FEC_CELL_TO_FACE_STENCIL_OBJECT_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_CENTRED_FEC_CELL_TO_FACE_STENCIL_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::centredFECCellToFaceStencilObject
// Description
// SourceFiles
#include "extended_centred_cell_to_face_stencil.hpp"
#include "fec_cell_to_face_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class centredFECCellToFaceStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    centredFECCellToFaceStencilObject
  >,
  public extendedCentredCellToFaceStencil
{
public:
  TYPE_NAME("centredFECCellToFaceStencil");
  // Constructors
    //- Construct from uncompacted face stencil
    explicit centredFECCellToFaceStencilObject
    (
      const fvMesh& mesh
    )
    :
      MeshObject
      <
        fvMesh,
        mousse::TopologicalMeshObject,
        centredFECCellToFaceStencilObject
      >(mesh),
      extendedCentredCellToFaceStencil(FECCellToFaceStencil(mesh))
    {
      if (extendedCellToFaceStencil::debug)
      {
        Info<< "Generated centred stencil " << type()
          << nl << endl;
        writeStencilStats(Info, stencil(), map());
      }
    }
  //- Destructor
  virtual ~centredFECCellToFaceStencilObject()
  {}
};
}  // namespace mousse
#endif
