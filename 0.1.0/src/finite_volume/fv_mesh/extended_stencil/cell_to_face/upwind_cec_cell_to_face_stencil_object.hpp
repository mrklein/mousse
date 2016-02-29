#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_UPWIND_CEC_CELL_TO_FACE_STENCIL_OBJECT_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_UPWIND_CEC_CELL_TO_FACE_STENCIL_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::upwindCECCellToFaceStencilObject
// Description
// SourceFiles
#include "extended_upwind_cell_to_face_stencil.hpp"
#include "cec_cell_to_face_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class upwindCECCellToFaceStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    upwindCECCellToFaceStencilObject
  >,
  public extendedUpwindCellToFaceStencil
{
public:
  TYPE_NAME("upwindCECCellToFaceStencil");
  // Constructors
    //- Construct from uncompacted face stencil
    upwindCECCellToFaceStencilObject
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
        upwindCECCellToFaceStencilObject
      >(mesh),
      extendedUpwindCellToFaceStencil
      (
        CECCellToFaceStencil(mesh),
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
  virtual ~upwindCECCellToFaceStencilObject()
  {}
};
}  // namespace mousse
#endif
