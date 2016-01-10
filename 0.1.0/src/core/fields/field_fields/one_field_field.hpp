// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::oneFieldField
// Description
//   A class representing the concept of a field of oneFields used to
//   avoid unnecessary manipulations for objects which are known to be one at
//   compile-time.
//   Used for example as the density argument to a function written for
//   compressible to be used for incompressible flow.
#ifndef one_field_field_hpp_
#define one_field_field_hpp_
#include "one_field.hpp"
namespace mousse
{
class oneFieldField
:
  public one
{
public:
  // Constructors
    //- Construct null
    oneFieldField()
    {}
  // Member Operators
    inline oneField operator[](const label) const;
};
}  // namespace mousse
#   include "one_field_field_i.hpp"
#endif
