#ifndef CORE_PRIMITIVES_BOOLS_BOOL_LIST_HPP_
#define CORE_PRIMITIVES_BOOLS_BOOL_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bool.hpp"
#include "list.hpp"
namespace mousse
{
  typedef UList<bool> boolUList;
  typedef List<bool> boolList;
  typedef List<List<bool> > boolListList;
}
#endif
