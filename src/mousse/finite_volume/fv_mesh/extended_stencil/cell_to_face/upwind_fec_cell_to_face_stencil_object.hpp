#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_UPWIND_FEC_CELL_TO_FACE_STENCIL_OBJECT_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_UPWIND_FEC_CELL_TO_FACE_STENCIL_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::upwindFECCellToFaceStencilObject
// Description
// SourceFiles
#include "extended_upwind_cell_to_face_stencil.hpp"
#include "fec_cell_to_face_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class upwindFECCellToFaceStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    upwindFECCellToFaceStencilObject
  >,
  public extendedUpwindCellToFaceStencil
{
public:
  TYPE_NAME("upwindFECCellToFaceStencil");
  // Constructors
    //- Construct from uncompacted face stencil
    upwindFECCellToFaceStencilObject
    (
      const fvMesh& mesh,
      const bool pureUpwind,
      const scalar minOpposedness
    )
    :
      MeshObject
      <
        fvMesh,
        mousse::TopologicalMeshObject,
        upwindFECCellToFaceStencilObject
      >(mesh),
      extendedUpwindCellToFaceStencil
      (
        FECCellToFaceStencil(mesh),
        pureUpwind,
        minOpposedness
      )
    {
      if (extendedCellToFaceStencil::debug)
      {
        Info<< "Generated off-centred stencil " << type()
          << nl << endl;
        writeStencilStats(Info, ownStencil(), ownMap());
      }
    }
  //- Destructor
  virtual ~upwindFECCellToFaceStencilObject()
  {}
};
}  // namespace mousse
#endif
