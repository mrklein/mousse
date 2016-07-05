#ifndef CORE_MESHES_MESH_SHAPES_EDGE_IO_LIST_HPP_
#define CORE_MESHES_MESH_SHAPES_EDGE_IO_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "edge.hpp"
#include "compact_io_list.hpp"


namespace mousse {

typedef IOList<edge> edgeIOList;
typedef CompactIOList<edge, label> edgeCompactIOList;

}

#endif
