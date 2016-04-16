// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "set_updater.hpp"
#include "poly_mesh.hpp"
#include "time.hpp"
#include "map_poly_mesh.hpp"
#include "ioobject_list.hpp"


// Member Functions 
template<class Type>
void mousse::setUpdater::updateSets(const mapPolyMesh& morphMap) const
{
  //
  // Update all sets in memory.
  //
  HashTable<const Type*> memSets =
    morphMap.mesh().objectRegistry::lookupClass<Type>();
  FOR_ALL_ITER(typename HashTable<const Type*>, memSets, iter) {
    Type& set = const_cast<Type&>(*iter());
    if (debug) {
      Pout << "Set:" << set.name() << " size:" << set.size()
        << " updated in memory" << endl;
    }
    set.updateMesh(morphMap);
    // Write or not? Debatable.
    set.write();
  }
  // Update all sets on disk
  // Get last valid mesh (discard points-only change)
  IOobjectList Objects
  {
    morphMap.mesh().time(),
    morphMap.mesh().facesInstance(),
    "polyMesh/sets"
  };
  IOobjectList fileSets{Objects.lookupClass(Type::typeName)};
  FOR_ALL_CONST_ITER(IOobjectList, fileSets, iter) {
    if (!memSets.found(iter.key())) {
      // Not in memory. Load it.
      Type set{*iter()};
      if (debug) {
        Pout << "Set:" << set.name() << " size:" << set.size()
          << " updated on disk" << endl;
      }
      set.updateMesh(morphMap);
      set.write();
    } else {
      if (debug) {
        Pout << "Set:" << iter.key() << " already updated from memory"
          << endl;
      }
    }
  }
}

