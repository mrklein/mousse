#ifndef CORE_PRIMITIVES_SCALAR_SCALAR_LIST_IO_LIST_HPP_
#define CORE_PRIMITIVES_SCALAR_SCALAR_LIST_IO_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar_list.hpp"
#include "compact_io_list.hpp"
namespace mousse
{
  typedef IOList<scalarList> scalarListIOList;
  typedef CompactIOList<scalarList, scalar> scalarListCompactIOList;
}
#endif
