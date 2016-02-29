#ifndef CORE_PRIMITIVES_TENSOR_LABEL_TENSOR_HPP_
#define CORE_PRIMITIVES_TENSOR_LABEL_TENSOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   label_tensor.cpp
#include "_tensor.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef Tensor<label> labelTensor;
//- Specify data associated with labelTensor type are contiguous
template<>
inline bool contiguous<labelTensor>() {return true;}
}  // namespace mousse
#endif
