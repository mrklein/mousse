// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef vector_list_hpp_
#define vector_list_hpp_
#include "vector.hpp"
#include "list.hpp"
namespace mousse
{
  typedef UList<vector> vectorUList;
  typedef List<vector> vectorList;
}
#endif
