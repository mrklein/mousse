#ifndef CORE_PRIMITIVES_INTS_LABEL_LIST_HPP_
#define CORE_PRIMITIVES_INTS_LABEL_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "label.hpp"
#include "list.hpp"

namespace mousse
{
  // Note: frequently used UList version is located in container itself
  typedef List<label> labelList;
  typedef List<labelList> labelListList;
  typedef List<labelListList> labelListListList;
}
#endif
