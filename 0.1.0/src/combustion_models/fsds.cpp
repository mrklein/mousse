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
makeCombustionTypesThermo
(
  FSD,
  psiThermoCombustion,
  gasHThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  FSD,
  psiThermoCombustion,
  constGasHThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  FSD,
  rhoThermoCombustion,
  gasHThermoPhysics,
  rhoCombustionModel
);
makeCombustionTypesThermo
(
  FSD,
  rhoThermoCombustion,
  constGasHThermoPhysics,
  rhoCombustionModel
);
// Combustion models based on sensibleInternalEnergy
makeCombustionTypesThermo
(
  FSD,
  psiThermoCombustion,
  gasEThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  FSD,
  psiThermoCombustion,
  constGasEThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  FSD,
  rhoThermoCombustion,
  gasEThermoPhysics,
  rhoCombustionModel
);
makeCombustionTypesThermo
(
  FSD,
  rhoThermoCombustion,
  constGasEThermoPhysics,
  rhoCombustionModel
);
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
