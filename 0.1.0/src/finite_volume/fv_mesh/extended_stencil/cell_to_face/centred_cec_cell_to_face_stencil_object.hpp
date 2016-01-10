// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::centredCECCellToFaceStencilObject
// Description
// SourceFiles
#ifndef centred_cec_cell_to_face_stencil_object_hpp_
#define centred_cec_cell_to_face_stencil_object_hpp_
#include "extended_centred_cell_to_face_stencil.hpp"
#include "cec_cell_to_face_stencil.hpp"
#include "_mesh_object.hpp"
namespace mousse
{
class centredCECCellToFaceStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    centredCECCellToFaceStencilObject
  >,
  public extendedCentredCellToFaceStencil
{
public:
  TYPE_NAME("centredCECCellToFaceStencil");
  // Constructors
    //- Construct from uncompacted face stencil
    explicit centredCECCellToFaceStencilObject
    (
      const fvMesh& mesh
    )
    :
      MeshObject
      <
        fvMesh,
        mousse::TopologicalMeshObject,
        centredCECCellToFaceStencilObject
      >(mesh),
      extendedCentredCellToFaceStencil(CECCellToFaceStencil(mesh))
    {
      if (extendedCellToFaceStencil::debug)
      {
        Info<< "Generated centred stencil " << type()
          << nl << endl;
        writeStencilStats(Info, stencil(), map());
      }
    }
  //- Destructor
  virtual ~centredCECCellToFaceStencilObject()
  {}
};
}  // namespace mousse
#endif
