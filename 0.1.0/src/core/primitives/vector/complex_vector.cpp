// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "complex_vector.hpp"


namespace mousse {

// Static Data Members
template<> const char* const complexVector::typeName = "complexVector";
template<>
const char* complexVector::componentNames[] = {"x", "y", "z"};

template<>
const complexVector complexVector::zero
{
  {0, 0},
  {0, 0},
  {0, 0}
};

template<>
const complexVector complexVector::one
{
  {1, 1},
  {1, 1},
  {1, 1}
};

}  // namespace mousse
