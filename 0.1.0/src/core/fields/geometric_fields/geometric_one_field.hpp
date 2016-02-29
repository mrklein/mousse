#ifndef CORE_FIELDS_GEOMETRIC_FIELDS_GEOMETRIC_ONE_FIELD_HPP_
#define CORE_FIELDS_GEOMETRIC_FIELDS_GEOMETRIC_ONE_FIELD_HPP_

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

inline const mousse::dimensionSet& mousse::geometricOneField::dimensions() const
{
  return dimless;
}
inline mousse::scalar mousse::geometricOneField::operator[](const label) const
{
  return scalar{1};
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
#endif
