#ifndef CORE_MESHES_MESH_SHAPES_PYRAMID_POINT_FACE_REF_HPP_
#define CORE_MESHES_MESH_SHAPES_PYRAMID_POINT_FACE_REF_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point.hpp"
#include "face.hpp"
#include "pyramid.hpp"


namespace mousse {

typedef pyramid<point, const point&, const face&> pyramidPointFaceRef;

}  // namespace mousse

#endif
