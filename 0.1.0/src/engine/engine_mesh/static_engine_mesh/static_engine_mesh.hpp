// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::staticEngineMesh
// Description
//   mousse::staticEngineMesh
// SourceFiles
//   static_engine_mesh.cpp
#ifndef static_engine_mesh_hpp_
#define static_engine_mesh_hpp_
#include "engine_mesh.hpp"
namespace mousse
{
class staticEngineMesh
:
  public engineMesh
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    staticEngineMesh(const staticEngineMesh&);
    //- Disallow default bitwise assignment
    void operator=(const staticEngineMesh&);
public:
  //- Runtime type information
  TypeName("static");
  // Constructors
    //- Construct from IOobject
    staticEngineMesh(const IOobject& io);
  //- Destructor
  ~staticEngineMesh();
  // Member Functions
    // Edit
      void move();
};
}  // namespace mousse
#endif
