// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   vector_2d.cpp
#ifndef vector_2d_hpp_
#define vector_2d_hpp_
#include "scalar.hpp"
#include "_vector_2d.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef Vector2D<scalar> vector2D;
//- Data associated with vector2D type are contiguous
template<>
inline bool contiguous<vector2D>() {return true;}
}  // namespace mousse
#endif
