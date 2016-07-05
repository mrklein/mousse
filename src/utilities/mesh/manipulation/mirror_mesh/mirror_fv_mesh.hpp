#ifndef UTILITIES_MESH_MANIPULATION_MIRROR_MESH_MIRROR_FV_MESH_HPP_
#define UTILITIES_MESH_MANIPULATION_MIRROR_MESH_MIRROR_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mirrorFvMesh

#include "fv_mesh.hpp"


namespace mousse {

class mirrorFvMesh
:
  public fvMesh
{
  // Private data
    //- Mirror dictionary
    IOdictionary mirrorMeshDict_;
    //- Mirrored mesh
    fvMesh* mirrorMeshPtr_;
public:
  // Constructors
    //- Construct from IOobject
    mirrorFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    mirrorFvMesh(const mirrorFvMesh&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const mirrorFvMesh&) = delete;
  //- Destructor
  ~mirrorFvMesh();
  // Member Functions
    //- Return reference to mirror mesh
    const fvMesh& mirrorMesh() const { return *mirrorMeshPtr_; }
};

}  // namespace mousse

#endif

