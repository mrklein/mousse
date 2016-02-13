// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mirrorFvMesh
// Description
// SourceFiles
//   mirror_fv_mesh.cpp
#ifndef mirror_fv_mesh_hpp_
#define mirror_fv_mesh_hpp_
#include "fv_mesh.hpp"
namespace mousse
{
class mirrorFvMesh
:
  public fvMesh
{
  // Private data
    //- Mirror dictionary
    IOdictionary mirrorMeshDict_;
    //- Mirrored mesh
    fvMesh* mirrorMeshPtr_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    mirrorFvMesh(const mirrorFvMesh&);
    //- Disallow default bitwise assignment
    void operator=(const mirrorFvMesh&);
public:
  // Constructors
    //- Construct from IOobject
    mirrorFvMesh(const IOobject& io);
  //- Destructor
  ~mirrorFvMesh();
  // Member Functions
    //- Return reference to mirror mesh
    const fvMesh& mirrorMesh() const
    {
      return *mirrorMeshPtr_;
    }
};
}  // namespace mousse
#endif
