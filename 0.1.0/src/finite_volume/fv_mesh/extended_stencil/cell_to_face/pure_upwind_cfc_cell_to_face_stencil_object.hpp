// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pureUpwindCFCCellToFaceStencilObject
// Description
// SourceFiles
#ifndef pure_upwind_cfc_cell_to_face_stencil_object_hpp_
#define pure_upwind_cfc_cell_to_face_stencil_object_hpp_
#include "extended_upwind_cell_to_face_stencil.hpp"
#include "cfc_cell_to_face_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class pureUpwindCFCCellToFaceStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    pureUpwindCFCCellToFaceStencilObject
  >,
  public extendedUpwindCellToFaceStencil
{
public:
  TYPE_NAME("pureUpwindCFCCellToFaceStencil");
  // Constructors
    //- Construct from uncompacted face stencil
    explicit pureUpwindCFCCellToFaceStencilObject
    (
      const fvMesh& mesh
    )
    :
      MeshObject
      <
        fvMesh,
        mousse::TopologicalMeshObject,
        pureUpwindCFCCellToFaceStencilObject
      >(mesh),
      extendedUpwindCellToFaceStencil(CFCCellToFaceStencil(mesh))
    {
      if (extendedCellToFaceStencil::debug)
      {
        Info<< "Generated pure upwind stencil " << type()
          << nl << endl;
        writeStencilStats(Info, ownStencil(), ownMap());
      }
    }
  //- Destructor
  virtual ~pureUpwindCFCCellToFaceStencilObject()
  {}
};
}  // namespace mousse
#endif
