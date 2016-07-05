// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_combustion_types.hpp"
#include "thermo_physics_types.hpp"
#include "psi_thermo_combustion.hpp"
#include "rho_thermo_combustion.hpp"
#include "diffusion.hpp"


// Combustion models based on sensibleEnthalpy
MAKE_COMBUSTION_TYPES_THERMO
(
  diffusion,
  psiThermoCombustion,
  gasHThermoPhysics,
  psiCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  diffusion,
  psiThermoCombustion,
  constGasHThermoPhysics,
  psiCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  diffusion,
  rhoThermoCombustion,
  gasHThermoPhysics,
  rhoCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  diffusion,
  rhoThermoCombustion,
  constGasHThermoPhysics,
  rhoCombustionModel
);

// Combustion models based on sensibleInternalEnergy
MAKE_COMBUSTION_TYPES_THERMO
(
  diffusion,
  psiThermoCombustion,
  gasEThermoPhysics,
  psiCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  diffusion,
  psiThermoCombustion,
  constGasEThermoPhysics,
  psiCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  diffusion,
  rhoThermoCombustion,
  gasEThermoPhysics,
  rhoCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  diffusion,
  rhoThermoCombustion,
  constGasEThermoPhysics,
  rhoCombustionModel
);

