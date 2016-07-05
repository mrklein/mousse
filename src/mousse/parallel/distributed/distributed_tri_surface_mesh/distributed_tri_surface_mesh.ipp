// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "distributed_tri_surface_mesh.hpp"
#include "tri_surface_fields.hpp"
#include "map_distribute.hpp"


// Member Functions 

template<class Type>
void mousse::distributedTriSurfaceMesh::distributeFields
(
  const mapDistribute& map
)
{
  typedef DimensionedField<Type, triSurfaceGeoMesh> DimensionedSurfField;
  typedef typename HashTable<DimensionedSurfField*>::iterator iterator;
  HashTable<DimensionedSurfField*> fields
  {
    objectRegistry::lookupClass<DimensionedSurfField>()
  };
  for (iterator fit = fields.begin(); fit != fields.end(); ++fit) {
    DimensionedSurfField& field = *fit();
    label oldSize = field.size();
    map.distribute(field);
    if (debug) {
      Info << "Mapped " << field.typeName << ' ' << field.name()
        << " from size " << oldSize << " to size " << field.size()
        << endl;
    }
  }
}

