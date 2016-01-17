// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef line_point_ref_hpp_
#define line_point_ref_hpp_
#include "point.hpp"
#include "line.hpp"
namespace mousse
{
typedef line<point, const point&> linePointRef;
}  // namespace mousse
#endif
