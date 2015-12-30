// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_chemistry_solver_types.hpp"
#include "thermo_physics_types.hpp"
#include "psi_chemistry_model.hpp"
#include "rho_chemistry_model.hpp"
namespace mousse
{
  // Chemistry solvers based on sensibleEnthalpy
  makeChemistrySolverTypes(psiChemistryModel, constGasHThermoPhysics);
  makeChemistrySolverTypes(psiChemistryModel, gasHThermoPhysics);
  makeChemistrySolverTypes
  (
    psiChemistryModel,
    constIncompressibleGasHThermoPhysics
  );
  makeChemistrySolverTypes
  (
    psiChemistryModel,
    incompressibleGasHThermoPhysics)
  ;
  makeChemistrySolverTypes(psiChemistryModel, icoPoly8HThermoPhysics);
  makeChemistrySolverTypes(rhoChemistryModel, constGasHThermoPhysics);
  makeChemistrySolverTypes(rhoChemistryModel, gasHThermoPhysics);
  makeChemistrySolverTypes
  (
    rhoChemistryModel,
    constIncompressibleGasHThermoPhysics
  );
  makeChemistrySolverTypes
  (
    rhoChemistryModel,
    incompressibleGasHThermoPhysics
  );
  makeChemistrySolverTypes(rhoChemistryModel, icoPoly8HThermoPhysics);
  // Chemistry solvers based on sensibleInternalEnergy
  makeChemistrySolverTypes(psiChemistryModel, constGasEThermoPhysics);
  makeChemistrySolverTypes(psiChemistryModel, gasEThermoPhysics);
  makeChemistrySolverTypes
  (
    psiChemistryModel,
    constIncompressibleGasEThermoPhysics
  );
  makeChemistrySolverTypes
  (
    psiChemistryModel,
    incompressibleGasEThermoPhysics
  );
  makeChemistrySolverTypes(psiChemistryModel, icoPoly8EThermoPhysics);
  makeChemistrySolverTypes(rhoChemistryModel, constGasEThermoPhysics);
  makeChemistrySolverTypes(rhoChemistryModel, gasEThermoPhysics);
  makeChemistrySolverTypes
  (
    rhoChemistryModel,
    constIncompressibleGasEThermoPhysics
  );
  makeChemistrySolverTypes
  (
    rhoChemistryModel,
    incompressibleGasEThermoPhysics
  );
  makeChemistrySolverTypes(rhoChemistryModel, icoPoly8EThermoPhysics);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
