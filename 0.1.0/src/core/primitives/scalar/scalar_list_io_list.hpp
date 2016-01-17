// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef scalar_list_io_list_hpp_
#define scalar_list_io_list_hpp_
#include "scalar_list.hpp"
#include "compact_io_list.hpp"
namespace mousse
{
  typedef IOList<scalarList> scalarListIOList;
  typedef CompactIOList<scalarList, scalar> scalarListCompactIOList;
}
#endif
