// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef tensor_list_hpp_
#define tensor_list_hpp_
#include "tensor.hpp"
#include "list.hpp"
namespace mousse
{
  typedef UList<tensor> tensorUList;
  typedef List<tensor> tensorList;
}
#endif
