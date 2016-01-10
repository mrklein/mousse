// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef scalar_list_hpp_
#define scalar_list_hpp_
#include "scalar.hpp"
#include "list.hpp"
namespace mousse
{
  typedef UList<scalar> scalarUList;
  typedef List<scalar> scalarList;
  typedef List<scalarList> scalarListList;
}
#endif
