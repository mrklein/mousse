#ifndef CORE_MESHES_MESH_SHAPES_CELL_IO_LIST_HPP_
#define CORE_MESHES_MESH_SHAPES_CELL_IO_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell.hpp"
#include "compact_io_list.hpp"
namespace mousse
{
  typedef IOList<cell> cellIOList;
  typedef CompactIOList<cell, label> cellCompactIOList;
}
#endif
