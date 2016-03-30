#ifndef CORE_FIELDS_CLOUD_MAP_CLOUDS_HPP_
#define CORE_FIELDS_CLOUD_MAP_CLOUDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cloud.hpp"
#include "object_registry.hpp"


namespace mousse {

//- Generic Geometric field mapper.
//  For "real" mapping, add template specialisations for mapping of internal
//  fields depending on mesh type.
inline void mapClouds(const objectRegistry& db, const mapPolyMesh& mapper)
{
  HashTable<const cloud*> clouds(db.lookupClass<cloud>());
  FOR_ALL_ITER(HashTable<const cloud*>, clouds, iter) {
    cloud& c = const_cast<cloud&>(*iter());
    if (polyMesh::debug) {
      Info << "Mapping cloud " << c.name() << endl;
    }
    c.autoMap(mapper);
  }
}
}  // namespace mousse
#endif
