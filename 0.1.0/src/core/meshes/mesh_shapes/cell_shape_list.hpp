// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef cell_shape_list_hpp_
#define cell_shape_list_hpp_
#include "cell_shape.hpp"
#include "list.hpp"
#include "ptr_list.hpp"
namespace mousse
{
  typedef List<cellShape> cellShapeList;
  typedef PtrList<cellShapeList> cellShapeListList;
}
#endif
