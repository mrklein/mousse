// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "geometric_zero_field.hpp"
inline const mousse::dimensionSet& mousse::geometricZeroField::dimensions() const
{
  return dimless;
}
inline mousse::scalar mousse::geometricZeroField::operator[](const label) const
{
  return scalar(0);
}
inline mousse::zeroField mousse::geometricZeroField::field() const
{
  return zeroField();
}
inline mousse::zeroField mousse::geometricZeroField::oldTime() const
{
  return zeroField();
}
inline mousse::zeroFieldField mousse::geometricZeroField::boundaryField() const
{
  return zeroFieldField();
}
