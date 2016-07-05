#ifndef CORE_CONTAINERS_HASH_TABLES_MAP_HPP_
#define CORE_CONTAINERS_HASH_TABLES_MAP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Map
// Description
//   A HashTable to objects of type \<T\> with a label key.
// See Also
//   PtrMap

#include "hash_table.hpp"
#include "label.hpp"
#include "hash.hpp"


namespace mousse {

template<class T>
class Map
:
  public HashTable<T, label, Hash<label>>
{
public:
  typedef typename HashTable<T, label, Hash<label>>::iterator iterator;
  typedef typename HashTable<T, label, Hash<label>>::const_iterator
    const_iterator;
  // Constructors
    //- Construct given initial size
    Map(const label size = 128)
    :
      HashTable<T, label, Hash<label>>(size)
    {}
    //- Construct from Istream
    Map(Istream& is)
    :
      HashTable<T, label, Hash<label>>(is)
    {}
    //- Construct as copy
    Map(const Map<T>& map)
    :
      HashTable<T, label, Hash<label>>(map)
    {}
    //- Construct by transferring the parameter contents
    Map(const Xfer<Map<T>>& map)
    :
      HashTable<T, label, Hash<label>>(map)
    {}
    //- Construct by transferring the parameter contents
    Map(const Xfer<HashTable<T, label, Hash<label>>>& map)
    :
      HashTable<T, label, Hash<label>>(map)
    {}
};
}  // namespace mousse
#endif
