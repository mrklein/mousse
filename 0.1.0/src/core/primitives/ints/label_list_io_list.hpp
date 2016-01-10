// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef label_list_io_list_hpp_
#define label_list_io_list_hpp_
#include "label_list.hpp"
#include "compact_io_list.hpp"
namespace mousse
{
  typedef IOList<labelList> labelListIOList;
  typedef CompactIOList<labelList, label> labelListCompactIOList;
}
#endif
