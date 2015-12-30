// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_search_mesh_object.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(meshSearchMeshObject, 0);
}
// Constructors 
mousse::meshSearchMeshObject::meshSearchMeshObject(const polyMesh& mesh)
:
  MeshObject<polyMesh, mousse::GeometricMeshObject, meshSearchMeshObject>(mesh),
  meshSearch(mesh)
{}
