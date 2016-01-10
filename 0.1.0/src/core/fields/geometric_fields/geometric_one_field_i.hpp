// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "geometric_one_field.hpp"
inline const mousse::dimensionSet& mousse::geometricOneField::dimensions() const
{
  return dimless;
}
inline mousse::scalar mousse::geometricOneField::operator[](const label) const
{
  return scalar(1);
}
inline mousse::oneField mousse::geometricOneField::field() const
{
  return oneField();
}
inline mousse::oneField mousse::geometricOneField::oldTime() const
{
  return oneField();
}
inline mousse::oneFieldField mousse::geometricOneField::boundaryField() const
{
  return oneFieldField();
}
inline const mousse::geometricOneField& mousse::operator*
(
  const geometricOneField& gof,
  const geometricOneField&
)
{
  return gof;
}
inline const mousse::geometricOneField& mousse::operator/
(
  const geometricOneField& gof,
  const geometricOneField&
)
{
  return gof;
}
