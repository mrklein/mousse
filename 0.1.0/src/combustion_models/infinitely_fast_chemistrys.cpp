// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_combustion_types.hpp"
#include "thermo_physics_types.hpp"
#include "psi_thermo_combustion.hpp"
#include "rho_thermo_combustion.hpp"
#include "infinitely_fast_chemistry.hpp"
// Combustion models based on sensibleEnthalpy
makeCombustionTypesThermo
(
  infinitelyFastChemistry,
  psiThermoCombustion,
  gasHThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  infinitelyFastChemistry,
  psiThermoCombustion,
  constGasHThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  infinitelyFastChemistry,
  rhoThermoCombustion,
  gasHThermoPhysics,
  rhoCombustionModel
);
makeCombustionTypesThermo
(
  infinitelyFastChemistry,
  rhoThermoCombustion,
  constGasHThermoPhysics,
  rhoCombustionModel
);
// Combustion models based on sensibleInternalEnergy
makeCombustionTypesThermo
(
  infinitelyFastChemistry,
  psiThermoCombustion,
  gasEThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  infinitelyFastChemistry,
  psiThermoCombustion,
  constGasEThermoPhysics,
  psiCombustionModel
);
makeCombustionTypesThermo
(
  infinitelyFastChemistry,
  rhoThermoCombustion,
  gasEThermoPhysics,
  rhoCombustionModel
);
makeCombustionTypesThermo
(
  infinitelyFastChemistry,
  rhoThermoCombustion,
  constGasEThermoPhysics,
  rhoCombustionModel
);
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
