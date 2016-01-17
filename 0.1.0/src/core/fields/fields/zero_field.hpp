// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::zeroField
// Description
//   A class representing the concept of a field of 0 used to avoid unnecessary
//   manipulations for objects which are known to be zero at compile-time.
//   Used for example as the argument to a function in which certain terms are
//   optional, see source terms in the MULES solvers.
#ifndef zero_field_hpp_
#define zero_field_hpp_
#include "zero.hpp"
#include "scalar.hpp"
namespace mousse
{
class zeroField
:
  public zero
{
public:
  // Constructors
    //- Construct null
    zeroField()
    {}
  // Member Operators
    inline scalar operator[](const label) const;
    inline zeroField field() const;
};
}  // namespace mousse

inline mousse::scalar mousse::zeroField::operator[](const label) const
{
  return scalar(0);
}
inline mousse::zeroField mousse::zeroField::field() const
{
  return zeroField();
}
#endif
