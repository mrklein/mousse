#ifndef CORE_PRIMITIVES_SCALAR_SCALAR_LIST_HPP_
#define CORE_PRIMITIVES_SCALAR_SCALAR_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar.hpp"
#include "list.hpp"
namespace mousse
{
  typedef UList<scalar> scalarUList;
  typedef List<scalar> scalarList;
  typedef List<scalarList> scalarListList;
}
#endif
