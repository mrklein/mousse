#ifndef CORE_FIELDS_FIELD_FIELDS_ZERO_FIELD_FIELD_HPP_
#define CORE_FIELDS_FIELD_FIELDS_ZERO_FIELD_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::zeroFieldField
// Description
//   A class representing the concept of a field of zeroFields used to
//   avoid unnecessary manipulations for objects which are known to be zero at
//   compile-time.
//   Used for example as the density argument to a function written for
//   compressible to be used for incompressible flow.
#include "zero_field.hpp"
namespace mousse
{
class zeroFieldField
:
  public zero
{
public:
  // Constructors
    //- Construct null
    zeroFieldField()
    {}
  // Member Operators
    inline zeroField operator[](const label) const;
};
}  // namespace mousse

inline mousse::zeroField mousse::zeroFieldField::operator[](const label) const
{
  return zeroField();
}
#endif
