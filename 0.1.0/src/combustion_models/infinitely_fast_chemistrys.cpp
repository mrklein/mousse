// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_combustion_types.hpp"
#include "thermo_physics_types.hpp"
#include "psi_thermo_combustion.hpp"
#include "rho_thermo_combustion.hpp"
#include "infinitely_fast_chemistry.hpp"
// Combustion models based on sensibleEnthalpy
MAKE_COMBUSTION_TYPES_THERMO
(
  infinitelyFastChemistry,
  psiThermoCombustion,
  gasHThermoPhysics,
  psiCombustionModel
);
MAKE_COMBUSTION_TYPES_THERMO
(
  infinitelyFastChemistry,
  psiThermoCombustion,
  constGasHThermoPhysics,
  psiCombustionModel
);
MAKE_COMBUSTION_TYPES_THERMO
(
  infinitelyFastChemistry,
  rhoThermoCombustion,
  gasHThermoPhysics,
  rhoCombustionModel
);
MAKE_COMBUSTION_TYPES_THERMO
(
  infinitelyFastChemistry,
  rhoThermoCombustion,
  constGasHThermoPhysics,
  rhoCombustionModel
);
// Combustion models based on sensibleInternalEnergy
MAKE_COMBUSTION_TYPES_THERMO
(
  infinitelyFastChemistry,
  psiThermoCombustion,
  gasEThermoPhysics,
  psiCombustionModel
);
MAKE_COMBUSTION_TYPES_THERMO
(
  infinitelyFastChemistry,
  psiThermoCombustion,
  constGasEThermoPhysics,
  psiCombustionModel
);
MAKE_COMBUSTION_TYPES_THERMO
(
  infinitelyFastChemistry,
  rhoThermoCombustion,
  gasEThermoPhysics,
  rhoCombustionModel
);
MAKE_COMBUSTION_TYPES_THERMO
(
  infinitelyFastChemistry,
  rhoThermoCombustion,
  constGasEThermoPhysics,
  rhoCombustionModel
);
