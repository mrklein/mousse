// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "zero_field.hpp"
inline mousse::scalar mousse::zeroField::operator[](const label) const
{
  return scalar(0);
}
inline mousse::zeroField mousse::zeroField::field() const
{
  return zeroField();
}
