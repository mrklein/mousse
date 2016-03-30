#ifndef CORE_FIELDS_FIELDS_ZERO_FIELD_HPP_
#define CORE_FIELDS_FIELDS_ZERO_FIELD_HPP_

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

#include "zero.hpp"
#include "scalar.hpp"


namespace mousse {

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
    inline scalar operator[](const label) const { return {0.0}; }
    inline zeroField field() const { return zeroField(); }
};
}  // namespace mousse

#endif
