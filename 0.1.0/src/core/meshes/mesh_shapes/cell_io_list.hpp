// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef cell_io_list_hpp_
#define cell_io_list_hpp_
#include "cell.hpp"
#include "compact_io_list.hpp"
namespace mousse
{
  typedef IOList<cell> cellIOList;
  typedef CompactIOList<cell, label> cellCompactIOList;
}
#endif
