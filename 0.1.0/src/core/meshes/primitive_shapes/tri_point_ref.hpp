// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef tri_point_ref_hpp_
#define tri_point_ref_hpp_
#include "point.hpp"
#include "triangle.hpp"
namespace mousse
{
typedef triangle<point, const point&> triPointRef;
}  // namespace mousse
#endif
