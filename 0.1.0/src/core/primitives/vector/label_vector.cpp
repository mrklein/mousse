// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_vector.hpp"
// Static Data Members
namespace mousse
{
  template<>
  const char* const mousse::labelVector::typeName = "labelVector";
  template<>
  const char* mousse::labelVector::componentNames[] = {"x", "y", "z"};
  template<>
  const mousse::labelVector mousse::labelVector::zero(0, 0, 0);
  template<>
  const mousse::labelVector mousse::labelVector::one(1, 1, 1);
}
