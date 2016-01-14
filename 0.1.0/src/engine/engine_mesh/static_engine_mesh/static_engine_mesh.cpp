// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "static_engine_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(staticEngineMesh, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(engineMesh, staticEngineMesh, IOobject);
}
// Constructors 
mousse::staticEngineMesh::staticEngineMesh(const IOobject& io)
:
  engineMesh(io)
{}
// Destructor 
mousse::staticEngineMesh::~staticEngineMesh()
{}
// Member Functions 
void mousse::staticEngineMesh::move()
{}
