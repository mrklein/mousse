// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_chemistry_model.hpp"
#include "psi_chemistry_model.hpp"
#include "chemistry_model.hpp"
#include "thermo_physics_types.hpp"
namespace mousse
{
// Chemistry moldels based on sensibleEnthalpy
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  psiChemistryModel,
  constGasHThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  psiChemistryModel,
  gasHThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  psiChemistryModel,
  constIncompressibleGasHThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  psiChemistryModel,
  incompressibleGasHThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  psiChemistryModel,
  icoPoly8HThermoPhysics
);
// Chemistry moldels based on sensibleInternalEnergy
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  psiChemistryModel,
  constGasEThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  psiChemistryModel,
  gasEThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  psiChemistryModel,
  constIncompressibleGasEThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  psiChemistryModel,
  incompressibleGasEThermoPhysics
);
MAKE_CHEMISTRY_MODEL
(
  chemistryModel,
  psiChemistryModel,
  icoPoly8EThermoPhysics
);
}
