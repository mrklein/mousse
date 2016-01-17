// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef label_pair_hpp_
#define label_pair_hpp_
#include "pair.hpp"
#include "list.hpp"
namespace mousse
{
  typedef Pair<label> labelPair;
  typedef List<labelPair> labelPairList;
}
#endif
