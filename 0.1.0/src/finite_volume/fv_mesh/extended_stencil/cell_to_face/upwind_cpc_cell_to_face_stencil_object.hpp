// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::upwindCPCCellToFaceStencilObject
// Description
// SourceFiles
#ifndef upwind_cpc_cell_to_face_stencil_object_hpp_
#define upwind_cpc_cell_to_face_stencil_object_hpp_
#include "extended_upwind_cell_to_face_stencil.hpp"
#include "cpc_cell_to_face_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class upwindCPCCellToFaceStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    upwindCPCCellToFaceStencilObject
  >,
  public extendedUpwindCellToFaceStencil
{
public:
  TypeName("upwindCPCCellToFaceStencil");
  // Constructors
    //- Construct from uncompacted face stencil
    upwindCPCCellToFaceStencilObject
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
        upwindCPCCellToFaceStencilObject
      >(mesh),
      extendedUpwindCellToFaceStencil
      (
        CPCCellToFaceStencil(mesh),
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
  virtual ~upwindCPCCellToFaceStencilObject()
  {}
};
}  // namespace mousse
#endif
