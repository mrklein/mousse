// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fluentFvMesh
// Description
// SourceFiles
//   fluent_fv_mesh.cpp
#ifndef fluent_fv_mesh_hpp_
#define fluent_fv_mesh_hpp_
#include "fv_mesh.hpp"
namespace mousse
{
class fluentFvMesh
:
  public fvMesh
{
public:
  // Constructors
    //- Construct from IOobject
    fluentFvMesh(const IOobject& io);
  // Member functions
    //- Write Fluent mesh
    void writeFluentMesh() const;
};
}  // namespace mousse
#endif
