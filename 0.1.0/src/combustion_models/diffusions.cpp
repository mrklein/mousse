// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_combustion_types.hpp"
#include "thermo_physics_types.hpp"
#include "psi_thermo_combustion.hpp"
#include "rho_thermo_combustion.hpp"
#include "diffusion.hpp"
// Combustion models based on sensibleEnthalpy
makeCombustionTypesThermo
(
  diffusion,
  psiThermoCombustion,
  gasHThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  diffusion,
  psiThermoCombustion,
  constGasHThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  diffusion,
  rhoThermoCombustion,
  gasHThermoPhysics,
  rhoCombustionModel
);
makeCombustionTypesThermo
(
  diffusion,
  rhoThermoCombustion,
  constGasHThermoPhysics,
  rhoCombustionModel
);
// Combustion models based on sensibleInternalEnergy
makeCombustionTypesThermo
(
  diffusion,
  psiThermoCombustion,
  gasEThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  diffusion,
  psiThermoCombustion,
  constGasEThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  diffusion,
  rhoThermoCombustion,
  gasEThermoPhysics,
  rhoCombustionModel
);
makeCombustionTypesThermo
(
  diffusion,
  rhoThermoCombustion,
  constGasEThermoPhysics,
  rhoCombustionModel
);
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
