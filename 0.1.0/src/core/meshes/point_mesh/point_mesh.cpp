// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_mesh.hpp"
#include "global_mesh_data.hpp"
#include "point_mesh_mapper.hpp"
#include "point_fields.hpp"
#include "map_geometric_fields.hpp"
#include "map_point_field.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(pointMesh, 0);

}


// Private Member Functions 
void mousse::pointMesh::mapFields(const mapPolyMesh& mpm)
{
  if (debug) {
    Pout << "void pointMesh::mapFields(const mapPolyMesh&): "
      << "Mapping all registered pointFields."
      << endl;
  }
  // Create a mapper
  const pointMeshMapper m{*this, mpm};
  MapGeometricFields<scalar, pointPatchField, pointMeshMapper, pointMesh>(m);
  MapGeometricFields<vector, pointPatchField, pointMeshMapper, pointMesh>(m);
  MapGeometricFields
  <
    sphericalTensor,
    pointPatchField,
    pointMeshMapper,
    pointMesh
  >(m);
  MapGeometricFields<symmTensor, pointPatchField, pointMeshMapper, pointMesh>
  (m);
  MapGeometricFields<tensor, pointPatchField, pointMeshMapper, pointMesh>(m);
}


// Constructors 
mousse::pointMesh::pointMesh(const polyMesh& pMesh)
:
  MeshObject<polyMesh, mousse::UpdateableMeshObject, pointMesh>{pMesh},
  GeoMesh<polyMesh>{pMesh},
  boundary_{*this, pMesh.boundaryMesh()}
{
  if (debug) {
    Pout << "pointMesh::pointMesh(const polyMesh&): "
      << "Constructing from polyMesh " << pMesh.name()
      << endl;
  }
  // Calculate the geometry for the patches (transformation tensors etc.)
  boundary_.calcGeometry();
}


// Destructor 
mousse::pointMesh::~pointMesh()
{
  if (debug) {
    Pout << "~pointMesh::pointMesh()"
      << endl;
  }
}


// Member Functions 
bool mousse::pointMesh::movePoints()
{
  if (debug) {
    Pout << "pointMesh::movePoints(const pointField&): "
      << "Moving points." << endl;
  }
  boundary_.movePoints(GeoMesh<polyMesh>::mesh_.points());
  return true;
}


void mousse::pointMesh::updateMesh(const mapPolyMesh& mpm)
{
  if (debug) {
    Pout << "pointMesh::updateMesh(const mapPolyMesh&): "
      << "Updating for topology changes." << endl;
    Pout << endl;
  }
  boundary_.updateMesh();
  // Map all registered point fields
  mapFields(mpm);
}

