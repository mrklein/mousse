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
  makeChemistryModel
  (
    chemistryModel,
    rhoChemistryModel,
    constGasHThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    rhoChemistryModel,
    gasHThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    rhoChemistryModel,
    constIncompressibleGasHThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    rhoChemistryModel,
    incompressibleGasHThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    rhoChemistryModel,
    icoPoly8HThermoPhysics
  );
  // Chemistry moldels based on sensibleInternalEnergy
  makeChemistryModel
  (
    chemistryModel,
    rhoChemistryModel,
    constGasEThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    rhoChemistryModel,
    gasEThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    rhoChemistryModel,
    constIncompressibleGasEThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    rhoChemistryModel,
    incompressibleGasEThermoPhysics
  );
  makeChemistryModel
  (
    chemistryModel,
    rhoChemistryModel,
    icoPoly8EThermoPhysics
  );
}
