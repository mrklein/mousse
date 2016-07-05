// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_chemistry_solver_types.hpp"
#include "thermo_physics_types.hpp"
#include "psi_chemistry_model.hpp"
#include "rho_chemistry_model.hpp"


namespace mousse {

// Chemistry solvers based on sensibleEnthalpy
MAKE_CHEMISTRY_SOLVER_TYPES(psiChemistryModel, constGasHThermoPhysics);
MAKE_CHEMISTRY_SOLVER_TYPES(psiChemistryModel, gasHThermoPhysics);
MAKE_CHEMISTRY_SOLVER_TYPES
(
  psiChemistryModel,
  constIncompressibleGasHThermoPhysics
);
MAKE_CHEMISTRY_SOLVER_TYPES
(
  psiChemistryModel,
  incompressibleGasHThermoPhysics
);
MAKE_CHEMISTRY_SOLVER_TYPES(psiChemistryModel, icoPoly8HThermoPhysics);
MAKE_CHEMISTRY_SOLVER_TYPES(rhoChemistryModel, constGasHThermoPhysics);
MAKE_CHEMISTRY_SOLVER_TYPES(rhoChemistryModel, gasHThermoPhysics);
MAKE_CHEMISTRY_SOLVER_TYPES
(
  rhoChemistryModel,
  constIncompressibleGasHThermoPhysics
);
MAKE_CHEMISTRY_SOLVER_TYPES
(
  rhoChemistryModel,
  incompressibleGasHThermoPhysics
);
MAKE_CHEMISTRY_SOLVER_TYPES(rhoChemistryModel, icoPoly8HThermoPhysics);
// Chemistry solvers based on sensibleInternalEnergy
MAKE_CHEMISTRY_SOLVER_TYPES(psiChemistryModel, constGasEThermoPhysics);
MAKE_CHEMISTRY_SOLVER_TYPES(psiChemistryModel, gasEThermoPhysics);
MAKE_CHEMISTRY_SOLVER_TYPES
(
  psiChemistryModel,
  constIncompressibleGasEThermoPhysics
);
MAKE_CHEMISTRY_SOLVER_TYPES
(
  psiChemistryModel,
  incompressibleGasEThermoPhysics
);
MAKE_CHEMISTRY_SOLVER_TYPES(psiChemistryModel, icoPoly8EThermoPhysics);
MAKE_CHEMISTRY_SOLVER_TYPES(rhoChemistryModel, constGasEThermoPhysics);
MAKE_CHEMISTRY_SOLVER_TYPES(rhoChemistryModel, gasEThermoPhysics);
MAKE_CHEMISTRY_SOLVER_TYPES
(
  rhoChemistryModel,
  constIncompressibleGasEThermoPhysics
);
MAKE_CHEMISTRY_SOLVER_TYPES
(
  rhoChemistryModel,
  incompressibleGasEThermoPhysics
);
MAKE_CHEMISTRY_SOLVER_TYPES(rhoChemistryModel, icoPoly8EThermoPhysics);

}

