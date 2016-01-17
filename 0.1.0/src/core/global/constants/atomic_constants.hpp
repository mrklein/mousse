// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef atomic_constants_hpp_
#define atomic_constants_hpp_
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
