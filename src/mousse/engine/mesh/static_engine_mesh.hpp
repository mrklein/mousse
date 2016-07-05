#ifndef ENGINE_ENGINE_MESH_STATIC_ENGINE_MESH_STATIC_ENGINE_MESH_HPP_
#define ENGINE_ENGINE_MESH_STATIC_ENGINE_MESH_STATIC_ENGINE_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::staticEngineMesh
// Description
//   mousse::staticEngineMesh

#include "engine_mesh.hpp"


namespace mousse {

class staticEngineMesh
:
  public engineMesh
{
public:
  //- Runtime type information
  TYPE_NAME("static");
  // Constructors
    //- Construct from IOobject
    staticEngineMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    staticEngineMesh(const staticEngineMesh&) = delete;
    //- Disallow default bitwise assignment
    staticEngineMesh& operator=(const staticEngineMesh&) = delete;
  //- Destructor
  ~staticEngineMesh();
  // Member Functions
    // Edit
      void move();
};

}  // namespace mousse

#endif

