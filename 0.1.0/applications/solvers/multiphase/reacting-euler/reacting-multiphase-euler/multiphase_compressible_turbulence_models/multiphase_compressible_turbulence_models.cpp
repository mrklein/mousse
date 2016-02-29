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
#include "smagorinsky.hpp"
MAKE_LES_MODEL(Smagorinsky);
#include "k_eqn.hpp"
MAKE_LES_MODEL(kEqn);
