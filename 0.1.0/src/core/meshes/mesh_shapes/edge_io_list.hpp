// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef edge_io_list_hpp_
#define edge_io_list_hpp_
#include "edge.hpp"
#include "compact_io_list.hpp"
namespace mousse
{
  typedef IOList<edge> edgeIOList;
  typedef CompactIOList<edge, label> edgeCompactIOList;
}
#endif
