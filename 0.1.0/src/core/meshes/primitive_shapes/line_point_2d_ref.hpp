// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef line_point_2d_ref_hpp_
#define line_point_2d_ref_hpp_
#include "point_2d.hpp"
#include "line.hpp"
namespace mousse
{
typedef line<point2D, const point2D&> linePoint2DRef;
}  // namespace mousse
#endif
