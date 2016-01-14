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
#ifndef zero_field_field_hpp_
#define zero_field_field_hpp_
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
#   include "zero_field_field_i.hpp"
#endif