#ifndef CORE_PRIMITIVES_SYMM_TENSOR_LABEL_SYMM_TENSOR_HPP_
#define CORE_PRIMITIVES_SYMM_TENSOR_LABEL_SYMM_TENSOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   label_symm_tensor.cpp
#include "_symm_tensor.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef SymmTensor<label> labelSymmTensor;
//- Data associated with labelSymmTensor type are contiguous
template<>
inline bool contiguous<labelSymmTensor>() {return true;}
}  // namespace mousse
#endif
