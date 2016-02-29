// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "euler_phase_compressible_turbulence_model.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "make_turbulence_model.hpp"
#include "laminar.hpp"
#include "ras_model.hpp"
#include "les_model.hpp"
MAKE_BASE_TURBULENCE_MODEL
(
  volScalarField,
  volScalarField,
  compressibleTurbulenceModel,
  PhaseCompressibleTurbulenceModel,
  ThermalDiffusivity,
  phaseModel
);
#define MAKE_RAS_MODEL(Type)                                                  \
  MAKE_TEMPLATED_TURBULENCE_MODEL                                             \
  (phaseModelPhaseCompressibleTurbulenceModel, RAS, Type)
#define MAKE_LES_MODEL(Type)                                                  \
  MAKE_TEMPLATED_TURBULENCE_MODEL                                             \
  (phaseModelPhaseCompressibleTurbulenceModel, LES, Type)
#include "k_epsilon.hpp"
MAKE_RAS_MODEL(kEpsilon);
#include "k_omega_sst.hpp"
MAKE_RAS_MODEL(kOmegaSST);
#include "k_omega_sst_sato.hpp"
MAKE_RAS_MODEL(kOmegaSSTSato);
#include "mixture_k_epsilon.hpp"
MAKE_RAS_MODEL(mixtureKEpsilon);
#include "lahey_k_epsilon.hpp"
MAKE_RAS_MODEL(LaheyKEpsilon);
#include "continuous_gas_k_epsilon.hpp"
MAKE_RAS_MODEL(continuousGasKEpsilon);
#include "smagorinsky.hpp"
MAKE_LES_MODEL(Smagorinsky);
#include "k_eqn.hpp"
MAKE_LES_MODEL(kEqn);
#include "smagorinsky_zhang.hpp"
MAKE_LES_MODEL(SmagorinskyZhang);
#include "niceno_k_eqn.hpp"
MAKE_LES_MODEL(NicenoKEqn);
#include "continuous_gas_k_eqn.hpp"
MAKE_LES_MODEL(continuousGasKEqn);
#include "kinetic_theory_model.hpp"
MAKE_TURBULENCE_MODEL
(phaseModelPhaseCompressibleTurbulenceModel, RAS, kineticTheoryModel);
#include "phase_pressure_model.hpp"
MAKE_TURBULENCE_MODEL
(phaseModelPhaseCompressibleTurbulenceModel, RAS, phasePressureModel);
