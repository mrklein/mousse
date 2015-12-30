// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::staticFvMesh
// Description
//   mousse::staticFvMesh
// SourceFiles
//   static_fv_mesh.cpp
#ifndef static_fv_mesh_hpp_
#define static_fv_mesh_hpp_
#include "dynamic_fv_mesh.hpp"
namespace mousse
{
class staticFvMesh
:
  public dynamicFvMesh
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    staticFvMesh(const staticFvMesh&);
    //- Disallow default bitwise assignment
    void operator=(const staticFvMesh&);
public:
  //- Runtime type information
  TypeName("staticFvMesh");
  // Constructors
    //- Construct from IOobject
    staticFvMesh(const IOobject& io);
  //- Destructor
  ~staticFvMesh();
  // Member Functions
    //- Dummy update function which does not change the mesh
    virtual bool update();
};
}  // namespace mousse
#endif
