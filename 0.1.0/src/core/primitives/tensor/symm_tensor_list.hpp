// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef symm_tensor_list_hpp_
#define symm_tensor_list_hpp_
#include "symm_tensor.hpp"
#include "list.hpp"
namespace mousse
{
  typedef UList<symmTensor> symmTensorUList;
  typedef List<symmTensor> symmTensorList;
}
#endif
