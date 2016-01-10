// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   spherical_tensor.cpp
#ifndef spherical_tensor_hpp_
#define spherical_tensor_hpp_
#include "_spherical_tensor.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef SphericalTensor<scalar> sphericalTensor;
// Identity tensor
static const sphericalTensor I(1);
static const sphericalTensor oneThirdI(1.0/3.0);
static const sphericalTensor twoThirdsI(2.0/3.0);
//- Specify data associated with sphericalTensor type are contiguous
template<>
inline bool contiguous<sphericalTensor>() {return true;}
}  // namespace mousse
#endif
