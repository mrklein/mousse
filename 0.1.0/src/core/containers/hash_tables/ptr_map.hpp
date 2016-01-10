// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PtrMap
// Description
//   A HashTable of pointers to objects of type \<T\> with a label key.
// See Also
//   Map, HashPtrTable
#ifndef ptr_map_hpp_
#define ptr_map_hpp_
#include "hash_ptr_table.hpp"
namespace mousse
{
template<class T>
class PtrMap
:
  public HashPtrTable<T, label, Hash<label> >
{
public:
  // Constructors
    //- Construct given initial map size
    PtrMap(const label size = 128)
    :
      HashPtrTable<T, label, Hash<label> >(size)
    {}
    //- Construct from Istream
    PtrMap(Istream& is)
    :
      HashPtrTable<T, label, Hash<label> >(is)
    {}
    //- Construct as copy
    PtrMap(const PtrMap<T>& map)
    :
      HashPtrTable<T, label, Hash<label> >(map)
    {}
};
}  // namespace mousse
#endif
