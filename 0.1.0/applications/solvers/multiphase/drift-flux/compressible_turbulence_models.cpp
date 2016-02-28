// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_compressible_turbulence_model.hpp"
#include "incompressible_two_phase_interacting_mixture.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "make_turbulence_model.hpp"
#include "laminar.hpp"
#include "ras_model.hpp"
#include "les_model.hpp"

MAKE_BASE_TURBULENCE_MODEL
(
  geometricOneField,
  volScalarField,
  compressibleTurbulenceModel,
  CompressibleTurbulenceModel,
  incompressibleTwoPhaseInteractingMixture
);

#define MAKE_RAS_MODEL(Type)                                                  \
  MAKE_TEMPLATED_TURBULENCE_MODEL                                             \
  (                                                                           \
    incompressibleTwoPhaseInteractingMixtureCompressibleTurbulenceModel,      \
    RAS,                                                                      \
    Type                                                                      \
  )
#define MAKE_LES_MODEL(Type)                                                  \
  MAKE_TEMPLATED_TURBULENCE_MODEL                                             \
  (                                                                           \
    incompressibleTwoPhaseInteractingMixtureCompressibleTurbulenceModel,      \
    LES,                                                                      \
    Type                                                                      \
  )
#include "k_epsilon.hpp"
MAKE_RAS_MODEL(kEpsilon);
#include "buoyant_k_epsilon.hpp"
MAKE_RAS_MODEL(buoyantKEpsilon);
#include "smagorinsky.hpp"
MAKE_LES_MODEL(Smagorinsky);
#include "k_eqn.hpp"
MAKE_LES_MODEL(kEqn);
