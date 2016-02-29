#ifndef CORE_PRIMITIVES_VECTOR_VECTOR_LIST_HPP_
#define CORE_PRIMITIVES_VECTOR_VECTOR_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vector.hpp"
#include "list.hpp"
namespace mousse
{
  typedef UList<vector> vectorUList;
  typedef List<vector> vectorList;
}
#endif
