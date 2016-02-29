#ifndef CORE_PRIMITIVES_PAIR_LABEL_PAIR_HPP_
#define CORE_PRIMITIVES_PAIR_LABEL_PAIR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pair.hpp"
#include "list.hpp"
namespace mousse
{
  typedef Pair<label> labelPair;
  typedef List<labelPair> labelPairList;
}
#endif
