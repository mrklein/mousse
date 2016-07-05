#ifndef CORE_MESHES_PRIMITIVE_SHAPES_POINT_INDEX_HIT_HPP_
#define CORE_MESHES_PRIMITIVE_SHAPES_POINT_INDEX_HIT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point.hpp"
#include "_point_index_hit.hpp"


namespace mousse {

typedef PointIndexHit<point> pointIndexHit;
//- Data associated with pointIndexHit type are contiguous
template<> 
  inline bool contiguous<pointIndexHit>() {return contiguous<point>();}

}

#endif
