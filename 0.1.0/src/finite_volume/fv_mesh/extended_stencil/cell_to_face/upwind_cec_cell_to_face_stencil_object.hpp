// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::upwindCECCellToFaceStencilObject
// Description
// SourceFiles
#ifndef upwind_cec_cell_to_face_stencil_object_hpp_
#define upwind_cec_cell_to_face_stencil_object_hpp_
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
