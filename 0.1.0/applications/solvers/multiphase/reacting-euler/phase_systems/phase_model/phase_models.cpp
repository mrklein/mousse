// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#include "rho_thermo.hpp"
#include "rho_reaction_thermo.hpp"
#include "rho_combustion_model.hpp"
#include "phase_model.hpp"
#include "thermo_phase_model.hpp"
#include "isothermal_phase_model.hpp"
#include "anisothermal_phase_model.hpp"
#include "pure_phase_model.hpp"
#include "multi_component_phase_model.hpp"
#include "inert_phase_model.hpp"
#include "reacting_phase_model.hpp"
#include "moving_phase_model.hpp"
namespace mousse
{
  typedef
    MovingPhaseModel
    <
      AnisothermalPhaseModel
      <
        PurePhaseModel
        <
          InertPhaseModel
          <
            ThermoPhaseModel<phaseModel, rhoThermo>
          >
        >
      >
    >
    purePhaseModel;
  ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
  (
    phaseModel,
    purePhaseModel,
    phaseSystem,
    purePhaseModel
  );
  typedef
    MovingPhaseModel
    <
      IsothermalPhaseModel
      <
        PurePhaseModel
        <
          InertPhaseModel
          <
            ThermoPhaseModel<phaseModel, rhoThermo>
          >
        >
      >
    >
    pureIsothermalPhaseModel;
  ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
  (
    phaseModel,
    pureIsothermalPhaseModel,
    phaseSystem,
    pureIsothermalPhaseModel
  );
  typedef
    MovingPhaseModel
    <
      AnisothermalPhaseModel
      <
        MultiComponentPhaseModel
        <
          InertPhaseModel
          <
            ThermoPhaseModel<phaseModel, rhoReactionThermo>
          >
        >
      >
    >
    multiComponentPhaseModel;
  ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
  (
    phaseModel,
    multiComponentPhaseModel,
    phaseSystem,
    multiComponentPhaseModel
  );
  typedef
    MovingPhaseModel
    <
      AnisothermalPhaseModel
      <
        MultiComponentPhaseModel
        <
          ReactingPhaseModel
          <
            ThermoPhaseModel<phaseModel, rhoReactionThermo>,
            combustionModels::rhoCombustionModel
          >
        >
      >
    >
    reactingPhaseModel;
  ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
  (
    phaseModel,
    reactingPhaseModel,
    phaseSystem,
    reactingPhaseModel
  );
}
