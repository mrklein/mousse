#ifndef CORE_PRIMITIVES_TENSOR_SPHERICAL_TENSOR_LIST_HPP_
#define CORE_PRIMITIVES_TENSOR_SPHERICAL_TENSOR_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spherical_tensor.hpp"
#include "list.hpp"
namespace mousse
{
  typedef UList<sphericalTensor> sphericalTensorUList;
  typedef List<sphericalTensor> sphericalTensorList;
}
#endif
