// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_combustion_types.hpp"
#include "thermo_physics_types.hpp"
#include "psi_combustion_model.hpp"
#include "psi_thermo_combustion.hpp"
#include "rho_combustion_model.hpp"
#include "rho_thermo_combustion.hpp"
#include "fsd.hpp"


// Combustion models based on sensibleEnthalpy
MAKE_COMBUSTION_TYPES_THERMO
(
  FSD,
  psiThermoCombustion,
  gasHThermoPhysics,
  psiCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  FSD,
  psiThermoCombustion,
  constGasHThermoPhysics,
  psiCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  FSD,
  rhoThermoCombustion,
  gasHThermoPhysics,
  rhoCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  FSD,
  rhoThermoCombustion,
  constGasHThermoPhysics,
  rhoCombustionModel
);

// Combustion models based on sensibleInternalEnergy
MAKE_COMBUSTION_TYPES_THERMO
(
  FSD,
  psiThermoCombustion,
  gasEThermoPhysics,
  psiCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  FSD,
  psiThermoCombustion,
  constGasEThermoPhysics,
  psiCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  FSD,
  rhoThermoCombustion,
  gasEThermoPhysics,
  rhoCombustionModel
);

MAKE_COMBUSTION_TYPES_THERMO
(
  FSD,
  rhoThermoCombustion,
  constGasEThermoPhysics,
  rhoCombustionModel
);

