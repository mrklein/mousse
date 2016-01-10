// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   spherical_tensor_2d.cpp
#ifndef spherical_tensor_2d_hpp_
#define spherical_tensor_2d_hpp_
#include "_spherical_tensor_2d.hpp"
#include "tensor.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef SphericalTensor2D<scalar> sphericalTensor2D;
//- Identity tensor
static const sphericalTensor2D I2D(1);
static const sphericalTensor2D oneThirdI2D(1.0/3.0);
static const sphericalTensor2D twoThirdsI2D(2.0/3.0);
//- Data associated with sphericalTensor2D type are contiguous
template<>
inline bool contiguous<sphericalTensor2D>() {return true;}
}  // namespace mousse
#endif
