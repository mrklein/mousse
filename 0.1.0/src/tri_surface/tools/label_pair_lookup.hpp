// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef label_pair_lookup_hpp_
#define label_pair_lookup_hpp_
#include "fixed_list.hpp"
#include "hash_table.hpp"
namespace mousse
{
  typedef HashTable
  <
   label,
   FixedList<label, 2>,
   FixedList<label, 2>::Hash<>
  > labelPairLookup;
}
#endif
