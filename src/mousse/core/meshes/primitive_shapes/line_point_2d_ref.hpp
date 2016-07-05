#ifndef CORE_MESHES_PRIMITIVE_SHAPES_LINE_POINT_2D_REF_HPP_
#define CORE_MESHES_PRIMITIVE_SHAPES_LINE_POINT_2D_REF_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_2d.hpp"
#include "line.hpp"


namespace mousse {

typedef line<point2D, const point2D&> linePoint2DRef;

}  // namespace mousse

#endif
