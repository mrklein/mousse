#ifndef CORE_PRIMITIVES_TENSOR_SYMM_TENSOR_LIST_HPP_
#define CORE_PRIMITIVES_TENSOR_SYMM_TENSOR_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symm_tensor.hpp"
#include "list.hpp"
namespace mousse
{
  typedef UList<symmTensor> symmTensorUList;
  typedef List<symmTensor> symmTensorList;
}
#endif
