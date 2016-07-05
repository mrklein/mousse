// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#include "phase_system.hpp"
#include "multiphase_system.hpp"
#include "momentum_transfer_phase_system.hpp"
#include "heat_transfer_phase_system.hpp"
#include "interface_composition_phase_change_phase_system.hpp"
#include "thermal_phase_change_phase_system.hpp"
namespace mousse
{
  typedef
    HeatTransferPhaseSystem
    <
      MomentumTransferPhaseSystem<multiphaseSystem>
    >
    heatAndMomentumTransferMultiphaseSystem;
  ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
  (
    multiphaseSystem,
    heatAndMomentumTransferMultiphaseSystem,
    dictionary,
    heatAndMomentumTransferMultiphaseSystem
  );
  typedef
    InterfaceCompositionPhaseChangePhaseSystem
    <
      MomentumTransferPhaseSystem<multiphaseSystem>
    >
    interfaceCompositionPhaseChangeMultiphaseSystem;
  ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
  (
    multiphaseSystem,
    interfaceCompositionPhaseChangeMultiphaseSystem,
    dictionary,
    interfaceCompositionPhaseChangeMultiphaseSystem
  );
  typedef
    ThermalPhaseChangePhaseSystem
    <
      MomentumTransferPhaseSystem<multiphaseSystem>
    >
    thermalPhaseChangeMultiphaseSystem;
  ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
  (
    multiphaseSystem,
    thermalPhaseChangeMultiphaseSystem,
    dictionary,
    thermalPhaseChangeMultiphaseSystem
  );
}
