// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "static_fv_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(staticFvMesh, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dynamicFvMesh, staticFvMesh, IOobject);

}


// Constructors 
mousse::staticFvMesh::staticFvMesh(const IOobject& io)
:
  dynamicFvMesh{io}
{}


// Destructor 
mousse::staticFvMesh::~staticFvMesh()
{}


// Member Functions 
bool mousse::staticFvMesh::update()
{
  return false;
}

