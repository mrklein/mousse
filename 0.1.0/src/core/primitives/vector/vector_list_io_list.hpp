// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef vector_list_io_list_hpp_
#define vector_list_io_list_hpp_
#include "vector_list.hpp"
#include "compact_io_list.hpp"
namespace mousse
{
  typedef IOList<vectorList> vectorListIOList;
  typedef CompactIOList<vectorList, vector> vectorListCompactIOList;
}
#endif
