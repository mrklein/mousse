// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_chemistry_model.hpp"
#include "rho_chemistry_model.hpp"
#include "chemistry_model.hpp"
#include "thermo_physics_types.hpp"
namespace mousse
{
// Chemistry moldels based on sensibleEnthalpy
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  rhoChemistryModel,
  constGasHThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  rhoChemistryModel,
  gasHThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  rhoChemistryModel,
  constIncompressibleGasHThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  rhoChemistryModel,
  incompressibleGasHThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  rhoChemistryModel,
  icoPoly8HThermoPhysics
);
// Chemistry moldels based on sensibleInternalEnergy
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  rhoChemistryModel,
  constGasEThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  rhoChemistryModel,
  gasEThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  rhoChemistryModel,
  constIncompressibleGasEThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  rhoChemistryModel,
  incompressibleGasEThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  rhoChemistryModel,
  icoPoly8EThermoPhysics
);
}
