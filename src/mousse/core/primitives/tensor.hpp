#ifndef CORE_PRIMITIVES_TENSOR_TENSOR_HPP_
#define CORE_PRIMITIVES_TENSOR_TENSOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   tensor.cpp

#include "_tensor.hpp"
#include "vector.hpp"
#include "spherical_tensor.hpp"
#include "symm_tensor.hpp"
#include "contiguous.hpp"


namespace mousse {

typedef Tensor<scalar> tensor;

vector eigenValues(const tensor&);
vector eigenVector(const tensor&, const scalar lambda);
tensor eigenVectors(const tensor&);
vector eigenValues(const symmTensor&);
vector eigenVector(const symmTensor&, const scalar lambda);
tensor eigenVectors(const symmTensor&);

//- Data associated with tensor type are contiguous
template<> inline bool contiguous<tensor>() {return true;}

}  // namespace mousse
#endif
