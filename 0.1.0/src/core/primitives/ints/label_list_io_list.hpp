#ifndef CORE_PRIMITIVES_INTS_LABEL_LIST_IO_LIST_HPP_
#define CORE_PRIMITIVES_INTS_LABEL_LIST_IO_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_list.hpp"
#include "compact_io_list.hpp"
namespace mousse
{
  typedef IOList<labelList> labelListIOList;
  typedef CompactIOList<labelList, label> labelListCompactIOList;
}
#endif
