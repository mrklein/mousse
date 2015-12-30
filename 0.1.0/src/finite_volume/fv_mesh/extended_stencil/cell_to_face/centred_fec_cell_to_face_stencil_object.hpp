// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::centredFECCellToFaceStencilObject
// Description
// SourceFiles
#ifndef centred_fec_cell_to_face_stencil_object_hpp_
#define centred_fec_cell_to_face_stencil_object_hpp_
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
  TypeName("centredFECCellToFaceStencil");
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
