#ifndef CORE_FIELDS_FIELDS_ONE_FIELD_HPP_
#define CORE_FIELDS_FIELDS_ONE_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::oneField
// Description
//   A class representing the concept of a field of 1 used to avoid unnecessary
//   manipulations for objects which are known to be one at compile-time.
//   Used for example as the density argument to a function written for
//   compressible to be used for incompressible flow.
#include "one.hpp"
#include "scalar.hpp"
namespace mousse
{
class oneField
:
  public one
{
public:
  // Constructors
    //- Construct null
    oneField()
    {}
  // Member Operators
    inline scalar operator[](const label) const;
    inline oneField field() const;
};
}  // namespace mousse

inline mousse::scalar mousse::oneField::operator[](const label) const
{
  return {1.0};
}
inline mousse::oneField mousse::oneField::field() const
{
  return oneField();
}
#endif
