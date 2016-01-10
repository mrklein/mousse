// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef spherical_tensor_list_hpp_
#define spherical_tensor_list_hpp_
#include "spherical_tensor.hpp"
#include "list.hpp"
namespace mousse
{
  typedef UList<sphericalTensor> sphericalTensorUList;
  typedef List<sphericalTensor> sphericalTensorList;
}
#endif
