#ifndef CORE_PRIMITIVES_TENSOR_TENSOR_LIST_HPP_
#define CORE_PRIMITIVES_TENSOR_TENSOR_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tensor.hpp"
#include "list.hpp"


namespace mousse {

typedef UList<tensor> tensorUList;
typedef List<tensor> tensorList;

}
#endif
