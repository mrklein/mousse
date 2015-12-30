// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "static_engine_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(staticEngineMesh, 0);
  addToRunTimeSelectionTable(engineMesh, staticEngineMesh, IOobject);
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
