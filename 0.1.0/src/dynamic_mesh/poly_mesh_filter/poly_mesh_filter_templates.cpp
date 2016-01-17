// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh_filter.hpp"
#include "poly_mesh.hpp"
#include "map_poly_mesh.hpp"
#include "ioobject_list.hpp"
// Public Member Functions
template<typename SetType>
void mousse::polyMeshFilter::updateSets(const mapPolyMesh& map)
{
  HashTable<const SetType*> sets =
    map.mesh().objectRegistry::lookupClass<const SetType>();
  FOR_ALL_ITER(typename HashTable<const SetType*>, sets, iter)
  {
    SetType& set = const_cast<SetType&>(*iter());
    set.updateMesh(map);
    set.sync(map.mesh());
  }
  IOobjectList Objects
  (
    map.mesh().time(),
    map.mesh().facesInstance(),
    "polyMesh/sets"
  );
  IOobjectList fileSets(Objects.lookupClass(SetType::typeName));
  FOR_ALL_CONST_ITER(IOobjectList, fileSets, iter)
  {
    if (!sets.found(iter.key()))
    {
      // Not in memory. Load it.
      SetType set(*iter());
      set.updateMesh(map);
      set.write();
    }
  }
}
template<typename SetType>
void mousse::polyMeshFilter::copySets
(
  const polyMesh& oldMesh,
  const polyMesh& newMesh
)
{
  HashTable<const SetType*> sets =
    oldMesh.objectRegistry::lookupClass<const SetType>();
  FOR_ALL_CONST_ITER(typename HashTable<const SetType*>, sets, iter)
  {
    const SetType& set = *iter();
    if (newMesh.objectRegistry::foundObject<SetType>(set.name()))
    {
      const SetType& origSet =
        newMesh.objectRegistry::lookupObject<SetType>(set.name());
      const_cast<SetType&>(origSet) = set;
      const_cast<SetType&>(origSet).sync(newMesh);
    }
    else
    {
      SetType* newSet
      (
        new SetType(newMesh, set.name(), set, set.writeOpt())
      );
      newSet->store();
      newSet->sync(newMesh);
    }
  }
}
