// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef bool_list_hpp_
#define bool_list_hpp_
#include "bool.hpp"
#include "list.hpp"
namespace mousse
{
  typedef UList<bool> boolUList;
  typedef List<bool> boolList;
  typedef List<List<bool> > boolListList;
}
#endif
