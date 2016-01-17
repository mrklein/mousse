// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   tensor2_d.cpp
#ifndef tensor_2d_hpp_
#define tensor_2d_hpp_
#include "_tensor_2d.hpp"
#include "_vector_2d.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef Tensor2D<scalar> tensor2D;
vector2D eigenValues(const tensor2D& t);
vector2D eigenVector(const tensor2D& t, const scalar lambda);
tensor2D eigenVectors(const tensor2D& t);
//- Data associated with tensor2D type are contiguous
template<>
inline bool contiguous<tensor2D>() {return true;}
}  // namespace mousse
#include "spherical_tensor_2d.hpp"
#endif
