// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_search_face_centre_tris_mesh_object.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(meshSearchFACE_CENTRE_TRISMeshObject, 0);
}
// Constructors 
mousse::meshSearchFACE_CENTRE_TRISMeshObject::meshSearchFACE_CENTRE_TRISMeshObject
(
  const polyMesh& mesh
)
:
  MeshObject
  <
    polyMesh,
    mousse::GeometricMeshObject,
    meshSearchFACE_CENTRE_TRISMeshObject
  >(mesh),
  meshSearch(mesh, polyMesh::FACE_CENTRE_TRIS)
{}
