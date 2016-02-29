#ifndef TRI_SURFACE_TOOLS_LABEL_PAIR_LOOKUP_HPP_
#define TRI_SURFACE_TOOLS_LABEL_PAIR_LOOKUP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

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
