#ifndef CORE_GLOBAL_CONSTANTS_UNIVERSAL_CONSTANTS_HPP_
#define CORE_GLOBAL_CONSTANTS_UNIVERSAL_CONSTANTS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dimensioned_scalar.hpp"
namespace mousse
{
namespace constant
{
namespace universal
{
  //- Group name for universal constants
  extern const char* const group;
  //- Reduced Planck constant: default SI units: [J/s]
  extern const dimensionedScalar hr;
}  // namespace universal
}  // namespace constant
}  // namespace mousse
#endif
