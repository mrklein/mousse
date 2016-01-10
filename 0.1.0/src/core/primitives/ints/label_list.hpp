// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef label_list_hpp_
#define label_list_hpp_

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
