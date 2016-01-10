// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "one_field.hpp"
inline mousse::scalar mousse::oneField::operator[](const label) const
{
  return scalar(1);
}
inline mousse::oneField mousse::oneField::field() const
{
  return oneField();
}
