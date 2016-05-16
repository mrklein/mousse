#ifndef UTILITIES_MESH_CONVERSION_FOAM_MESH_TO_FLUENT_FLUENT_FV_MESH_HPP_
#define UTILITIES_MESH_CONVERSION_FOAM_MESH_TO_FLUENT_FLUENT_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fluentFvMesh

#include "fv_mesh.hpp"


namespace mousse {

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

