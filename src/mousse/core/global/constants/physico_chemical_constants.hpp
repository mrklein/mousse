#ifndef CORE_GLOBAL_CONSTANTS_PHYSICO_CHEMICAL_CONSTANTS_HPP_
#define CORE_GLOBAL_CONSTANTS_PHYSICO_CHEMICAL_CONSTANTS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dimensioned_scalar.hpp"
namespace mousse
{
namespace constant
{
namespace physicoChemical
{
  //- Group name for physico-chemical constants
  extern const char* const group;
  //- Universal gas constant: default SI units: [J/mol/K]
  extern const dimensionedScalar R;
  //- Faraday constant: default SI units: [C/mol]
  extern const dimensionedScalar F;
  //- Stefan-Boltzmann constant: default SI units: [W/m2/K4]
  extern const dimensionedScalar sigma;
  //- Wien displacement law constant: default SI units: [m.K]
  extern const dimensionedScalar b;
  //- First radiation constant: default SI units: [W/m2]
  extern const dimensionedScalar c1;
  //- Second radiation constant: default SI units: [m.K]
  extern const dimensionedScalar c2;
}  // namespace physicoChemical
}  // namespace constant
}  // namespace mousse
#endif
