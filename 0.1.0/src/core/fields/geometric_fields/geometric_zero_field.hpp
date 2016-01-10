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
#ifndef geometric_zero_field_hpp_
#define geometric_zero_field_hpp_
#include "zero_field_field.hpp"
#include "dimension_set.hpp"
#include "scalar.hpp"
namespace mousse
{
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
#   include "geometric_zero_field_i.hpp"
#endif
