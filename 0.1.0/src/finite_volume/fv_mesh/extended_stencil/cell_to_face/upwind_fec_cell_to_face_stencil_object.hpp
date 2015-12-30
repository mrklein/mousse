// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::upwindFECCellToFaceStencilObject
// Description
// SourceFiles
#ifndef upwind_fec_cell_to_face_stencil_object_hpp_
#define upwind_fec_cell_to_face_stencil_object_hpp_
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
  TypeName("upwindFECCellToFaceStencil");
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
