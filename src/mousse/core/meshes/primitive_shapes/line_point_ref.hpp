#ifndef CORE_MESHES_PRIMITIVE_SHAPES_LINE_POINT_REF_HPP_
#define CORE_MESHES_PRIMITIVE_SHAPES_LINE_POINT_REF_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point.hpp"
#include "line.hpp"


namespace mousse {

typedef line<point, const point&> linePointRef;

}  // namespace mousse

#endif
