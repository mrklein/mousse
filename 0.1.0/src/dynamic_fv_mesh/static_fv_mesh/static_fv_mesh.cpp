// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "static_fv_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(staticFvMesh, 0);
  addToRunTimeSelectionTable(dynamicFvMesh, staticFvMesh, IOobject);
}
// Constructors 
mousse::staticFvMesh::staticFvMesh(const IOobject& io)
:
  dynamicFvMesh(io)
{}
// Destructor 
mousse::staticFvMesh::~staticFvMesh()
{}
// Member Functions 
bool mousse::staticFvMesh::update()
{
  return false;
}
