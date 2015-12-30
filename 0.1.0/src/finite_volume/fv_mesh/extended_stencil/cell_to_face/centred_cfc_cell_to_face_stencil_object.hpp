// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::centredCFCCellToFaceStencilObject
// Description
// SourceFiles
#ifndef centred_cfc_cell_to_face_stencil_object_hpp_
#define centred_cfc_cell_to_face_stencil_object_hpp_
#include "extended_centred_cell_to_face_stencil.hpp"
#include "cfc_cell_to_face_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class centredCFCCellToFaceStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    centredCFCCellToFaceStencilObject
  >,
  public extendedCentredCellToFaceStencil
{
public:
  TypeName("centredCFCCellToFaceStencil");
  // Constructors
    //- Construct from uncompacted face stencil
    explicit centredCFCCellToFaceStencilObject
    (
      const fvMesh& mesh
    )
    :
      MeshObject
      <
        fvMesh,
        mousse::TopologicalMeshObject,
        centredCFCCellToFaceStencilObject
      >(mesh),
      extendedCentredCellToFaceStencil(CFCCellToFaceStencil(mesh))
    {
      if (extendedCellToFaceStencil::debug)
      {
        Info<< "Generated centred stencil " << type()
          << nl << endl;
        writeStencilStats(Info, stencil(), map());
      }
    }
  //- Destructor
  virtual ~centredCFCCellToFaceStencilObject()
  {}
};
}  // namespace mousse
#endif
