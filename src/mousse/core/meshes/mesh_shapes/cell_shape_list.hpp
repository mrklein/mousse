#ifndef CORE_MESHES_MESH_SHAPES_CELL_SHAPE_LIST_HPP_
#define CORE_MESHES_MESH_SHAPES_CELL_SHAPE_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_shape.hpp"
#include "list.hpp"
#include "ptr_list.hpp"


namespace mousse {

typedef List<cellShape> cellShapeList;
typedef PtrList<cellShapeList> cellShapeListList;

}
#endif
