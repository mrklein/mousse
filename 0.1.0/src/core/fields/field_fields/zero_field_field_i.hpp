// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "zero_field_field.hpp"
inline mousse::zeroField mousse::zeroFieldField::operator[](const label) const
{
  return zeroField();
}
