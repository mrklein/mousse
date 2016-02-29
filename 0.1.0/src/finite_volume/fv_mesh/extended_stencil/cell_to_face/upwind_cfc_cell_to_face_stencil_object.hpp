#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_UPWIND_CFC_CELL_TO_FACE_STENCIL_OBJECT_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_UPWIND_CFC_CELL_TO_FACE_STENCIL_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::upwindCFCCellToFaceStencilObject
// Description
// SourceFiles
#include "extended_upwind_cell_to_face_stencil.hpp"
#include "cfc_cell_to_face_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class upwindCFCCellToFaceStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    upwindCFCCellToFaceStencilObject
  >,
  public extendedUpwindCellToFaceStencil
{
public:
  TYPE_NAME("upwindCFCCellToFaceStencil");
  // Constructors
    //- Construct from uncompacted face stencil
    upwindCFCCellToFaceStencilObject
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
        upwindCFCCellToFaceStencilObject
      >(mesh),
      extendedUpwindCellToFaceStencil
      (
        CFCCellToFaceStencil(mesh),
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
  virtual ~upwindCFCCellToFaceStencilObject()
  {}
};
}  // namespace mousse
#endif
