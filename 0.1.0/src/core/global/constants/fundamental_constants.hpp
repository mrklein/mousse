#ifndef CORE_GLOBAL_CONSTANTS_FUNDAMENTAL_CONSTANTS_HPP_
#define CORE_GLOBAL_CONSTANTS_FUNDAMENTAL_CONSTANTS_HPP_

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
  //- Speed of light in a vacuum
  extern const dimensionedScalar c;
  //- Newtonian constant of gravitation
  extern const dimensionedScalar G;
  //- Planck constant
  extern const dimensionedScalar h;
}
namespace electromagnetic
{
  //- Elementary charge
  extern const dimensionedScalar e;
}
namespace atomic
{
  //- Electron mass
  extern const dimensionedScalar me;
  //- Proton mass
  extern const dimensionedScalar mp;
}
namespace physicoChemical
{
  //- Atomic mass unit
  extern const dimensionedScalar mu;
  //- Avagadro number
  extern const dimensionedScalar NA;
  //- Boltzmann constant
  extern const dimensionedScalar k;
}
namespace standard
{
  //- Standard pressure
  extern const dimensionedScalar Pstd;
  //- Standard temperature
  extern const dimensionedScalar Tstd;
}
}  // namespace constant
}  // namespace mousse
#endif
