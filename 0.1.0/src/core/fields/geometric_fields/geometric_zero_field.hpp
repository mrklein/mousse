#ifndef CORE_FIELDS_GEOMETRIC_FIELDS_GEOMETRIC_ZERO_FIELD_HPP_
#define CORE_FIELDS_GEOMETRIC_FIELDS_GEOMETRIC_ZERO_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::geometricZeroField
// Description
//   A class representing the concept of a GeometricField of 1 used to avoid
//   unnecessary manipulations for objects which are known to be zero at
//   compile-time.
//   Used for example as the density argument to a function written for
//   compressible to be used for incompressible flow.

#include "zero_field_field.hpp"
#include "dimension_set.hpp"
#include "scalar.hpp"


namespace mousse {

class geometricZeroField
:
  public zero
{
public:
  // Constructors
    //- Construct null
    geometricZeroField()
    {}
  // Member Operators
    inline const dimensionSet& dimensions() const;
    inline scalar operator[](const label) const;
    inline zeroField field() const;
    inline zeroField oldTime() const;
    inline zeroFieldField boundaryField() const;
};
}  // namespace mousse

inline const mousse::dimensionSet& mousse::geometricZeroField::dimensions() const
{
  return dimless;
}


inline mousse::scalar mousse::geometricZeroField::operator[](const label) const
{
  return scalar{0};
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

#endif
