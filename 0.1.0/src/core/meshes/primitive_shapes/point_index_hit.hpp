// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef point_index_hit_hpp_
#define point_index_hit_hpp_
#include "point.hpp"
#include "_point_index_hit.hpp"
namespace mousse
{
typedef PointIndexHit<point> pointIndexHit;
//- Data associated with pointIndexHit type are contiguous
template<>
inline bool contiguous<pointIndexHit>() {return contiguous<point>();}
}
#endif
