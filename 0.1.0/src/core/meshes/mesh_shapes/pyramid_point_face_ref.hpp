// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef pyramid_point_face_ref_hpp_
#define pyramid_point_face_ref_hpp_
#include "point.hpp"
#include "face.hpp"
#include "pyramid.hpp"
namespace mousse
{
typedef pyramid<point, const point&, const face&> pyramidPointFaceRef;
}  // namespace mousse
#endif
