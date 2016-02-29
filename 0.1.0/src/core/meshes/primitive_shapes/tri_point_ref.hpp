#ifndef CORE_MESHES_PRIMITIVE_SHAPES_TRI_POINT_REF_HPP_
#define CORE_MESHES_PRIMITIVE_SHAPES_TRI_POINT_REF_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point.hpp"
#include "triangle.hpp"
namespace mousse
{
typedef triangle<point, const point&> triPointRef;
}  // namespace mousse
#endif
