// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_incompressible_turbulence_model.hpp"
#include "single_phase_transport_model.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "make_turbulence_model.hpp"
#include "laminar.hpp"
#include "turbulent_transport_model.hpp"
#include "les_model.hpp"

MAKE_BASE_TURBULENCE_MODEL
(
  volScalarField,
  geometricOneField,
  incompressibleTurbulenceModel,
  PhaseIncompressibleTurbulenceModel,
  singlePhaseTransportModel
);

#define MAKE_RAS_MODEL(Type)                                                  \
  MAKE_TEMPLATED_TURBULENCE_MODEL                                             \
  (singlePhaseTransportModelPhaseIncompressibleTurbulenceModel, RAS, Type)
#define MAKE_LES_MODEL(Type)                                                  \
  MAKE_TEMPLATED_TURBULENCE_MODEL                                             \
  (singlePhaseTransportModelPhaseIncompressibleTurbulenceModel, LES, Type)
#include "k_epsilon.hpp"
MAKE_RAS_MODEL(kEpsilon);
#include "smagorinsky.hpp"
MAKE_LES_MODEL(Smagorinsky);
#include "k_eqn.hpp"
MAKE_LES_MODEL(kEqn);
