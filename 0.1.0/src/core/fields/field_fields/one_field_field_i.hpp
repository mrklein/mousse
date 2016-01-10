// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "one_field_field.hpp"
inline mousse::oneField mousse::oneFieldField::operator[](const label) const
{
  return oneField();
}
