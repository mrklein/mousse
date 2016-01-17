// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::centredCPCCellToFaceStencilObject
// Description
// SourceFiles

#ifndef centred_cpc_cell_to_face_stencil_object_hpp_
#define centred_cpc_cell_to_face_stencil_object_hpp_

#include "extended_centred_cell_to_face_stencil.hpp"
#include "cpc_cell_to_face_stencil.hpp"
#include "_mesh_object.hpp"

namespace mousse
{
class centredCPCCellToFaceStencilObject
:
  public MeshObject
  <
    fvMesh,
    TopologicalMeshObject,
    centredCPCCellToFaceStencilObject
  >,
  public extendedCentredCellToFaceStencil
{
public:
  TYPE_NAME("centredCPCCellToFaceStencil");

  // Constructors
    //- Construct from uncompacted face stencil
    explicit centredCPCCellToFaceStencilObject
    (
      const fvMesh& mesh
    )
    :
      MeshObject
      <
        fvMesh,
        mousse::TopologicalMeshObject,
        centredCPCCellToFaceStencilObject
      >{mesh},
      extendedCentredCellToFaceStencil{CPCCellToFaceStencil{mesh}}
    {
      if (extendedCellToFaceStencil::debug)
      {
        Info<< "Generated centred stencil " << type()
          << nl << endl;
        writeStencilStats(Info, stencil(), map());
      }
    }
  //- Destructor
  virtual ~centredCPCCellToFaceStencilObject()
  {}
};

}  // namespace mousse
#endif
