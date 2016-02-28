// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#include "phase_system.hpp"
#include "two_phase_system.hpp"
#include "momentum_transfer_phase_system.hpp"
#include "heat_transfer_phase_system.hpp"
#include "interface_composition_phase_change_phase_system.hpp"
#include "thermal_phase_change_phase_system.hpp"
namespace mousse
{
  typedef
    HeatTransferPhaseSystem
    <
      MomentumTransferPhaseSystem<twoPhaseSystem>
    >
    heatAndMomentumTransferTwoPhaseSystem;
  ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
  (
    twoPhaseSystem,
    heatAndMomentumTransferTwoPhaseSystem,
    dictionary,
    heatAndMomentumTransferTwoPhaseSystem
  );
  typedef
    InterfaceCompositionPhaseChangePhaseSystem
    <
      MomentumTransferPhaseSystem<twoPhaseSystem>
    >
    interfaceCompositionPhaseChangeTwoPhaseSystem;
  ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
  (
    twoPhaseSystem,
    interfaceCompositionPhaseChangeTwoPhaseSystem,
    dictionary,
    interfaceCompositionPhaseChangeTwoPhaseSystem
  );
  typedef
    ThermalPhaseChangePhaseSystem
    <
      MomentumTransferPhaseSystem<twoPhaseSystem>
    >
    thermalPhaseChangeTwoPhaseSystem;
  ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
  (
    twoPhaseSystem,
    thermalPhaseChangeTwoPhaseSystem,
    dictionary,
    thermalPhaseChangeTwoPhaseSystem
  );
}
