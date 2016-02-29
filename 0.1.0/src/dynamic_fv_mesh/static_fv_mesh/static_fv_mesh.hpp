#ifndef DYNAMIC_FV_MESH_STATIC_FV_MESH_STATIC_FV_MESH_HPP_
#define DYNAMIC_FV_MESH_STATIC_FV_MESH_STATIC_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::staticFvMesh
// Description
//   mousse::staticFvMesh
// SourceFiles
//   static_fv_mesh.cpp
#include "dynamic_fv_mesh.hpp"
namespace mousse
{
class staticFvMesh
:
  public dynamicFvMesh
{
public:
  //- Runtime type information
  TYPE_NAME("staticFvMesh");
  // Constructors
    //- Construct from IOobject
    staticFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    staticFvMesh(const staticFvMesh&) = delete;
    //- Disallow default bitwise assignment
    staticFvMesh& operator=(const staticFvMesh&) = delete;
  //- Destructor
  ~staticFvMesh();
  // Member Functions
    //- Dummy update function which does not change the mesh
    virtual bool update();
};
}  // namespace mousse
#endif
