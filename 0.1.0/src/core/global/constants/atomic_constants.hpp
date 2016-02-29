#ifndef CORE_GLOBAL_CONSTANTS_ATOMIC_CONSTANTS_HPP_
#define CORE_GLOBAL_CONSTANTS_ATOMIC_CONSTANTS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dimensioned_scalar.hpp"
namespace mousse
{
namespace constant
{
namespace atomic
{
  //- Group name for atomic constants
  extern const char* const group;
  //- Fine-structure constant: default SI units: []
  extern const dimensionedScalar alpha;
  //- Rydberg constant: default SI units: [1/m]
  extern const dimensionedScalar Rinf;
  //- Bohr radius: default SI units: [m]
  extern const dimensionedScalar a0;
  //- Classical electron radius: default SI units: [m]
  extern const dimensionedScalar re;
  //- Hartree energy: default SI units: [J]
  extern const dimensionedScalar Eh;
}  // namespace atomic
}  // namespace constant
}  // namespace mousse
#endif
