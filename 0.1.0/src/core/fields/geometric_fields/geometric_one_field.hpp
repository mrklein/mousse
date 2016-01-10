// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::geometricOneField
// Description
//   A class representing the concept of a GeometricField of 1 used to avoid
//   unnecessary manipulations for objects which are known to be one at
//   compile-time.
//   Used for example as the density argument to a function written for
//   compressible to be used for incompressible flow.
#ifndef geometric_one_field_hpp_
#define geometric_one_field_hpp_
#include "one_field_field.hpp"
#include "dimension_set.hpp"
#include "scalar.hpp"
namespace mousse
{
class geometricOneField
:
  public one
{
public:
  // Constructors
    //- Construct null
    geometricOneField()
    {}
  // Member Operators
    inline const dimensionSet& dimensions() const;
    inline scalar operator[](const label) const;
    inline oneField field() const;
    inline oneField oldTime() const;
    inline oneFieldField boundaryField() const;
};
inline const geometricOneField& operator*
(
  const geometricOneField&,
  const geometricOneField&
);
inline const geometricOneField& operator/
(
  const geometricOneField&,
  const geometricOneField&
);
}  // namespace mousse
#   include "geometric_one_field_i.hpp"
#endif
