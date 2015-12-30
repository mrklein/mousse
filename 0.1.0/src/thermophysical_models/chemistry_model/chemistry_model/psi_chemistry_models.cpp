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
  makeChemistryModel
  (
    chemistryModel,
    psiChemistryModel,
    constGasHThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    psiChemistryModel,
    gasHThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    psiChemistryModel,
    constIncompressibleGasHThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    psiChemistryModel,
    incompressibleGasHThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    psiChemistryModel,
    icoPoly8HThermoPhysics
  );
  // Chemistry moldels based on sensibleInternalEnergy
  makeChemistryModel
  (
    chemistryModel,
    psiChemistryModel,
    constGasEThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    psiChemistryModel,
    gasEThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    psiChemistryModel,
    constIncompressibleGasEThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    psiChemistryModel,
    incompressibleGasEThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    psiChemistryModel,
    icoPoly8EThermoPhysics
  );
}
