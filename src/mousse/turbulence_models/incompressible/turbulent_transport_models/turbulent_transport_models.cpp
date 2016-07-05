// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_incompressible_turbulence_model.hpp"
#include "incompressible/transport_model.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "make_turbulence_model.hpp"
#include "laminar.hpp"
#include "ras_model.hpp"
#include "les_model.hpp"


MAKE_BASE_TURBULENCE_MODEL
(
  geometricOneField,
  geometricOneField,
  incompressibleTurbulenceModel,
  IncompressibleTurbulenceModel,
  transportModel
);


#define MAKE_RAS_MODEL(Type)                                                  \
  MAKE_TEMPLATED_TURBULENCE_MODEL                                             \
  (transportModelIncompressibleTurbulenceModel, RAS, Type)


#define MAKE_LES_MODEL(Type)                                                  \
  MAKE_TEMPLATED_TURBULENCE_MODEL                                             \
  (transportModelIncompressibleTurbulenceModel, LES, Type)


// RAS models
#include "spalart_allmaras.hpp"
MAKE_RAS_MODEL(SpalartAllmaras);
#include "k_epsilon.hpp"
MAKE_RAS_MODEL(kEpsilon);
#include "rng_k_epsilon.hpp"
MAKE_RAS_MODEL(RNGkEpsilon);
#include "realizable_ke.hpp"
MAKE_RAS_MODEL(realizableKE);
#include "launder_sharma_ke.hpp"
MAKE_RAS_MODEL(LaunderSharmaKE);
#include "k_omega.hpp"
MAKE_RAS_MODEL(kOmega);
#include "k_omega_sst.hpp"
MAKE_RAS_MODEL(kOmegaSST);
#include "k_omega_sstsas.hpp"
MAKE_RAS_MODEL(kOmegaSSTSAS);
#include "v2f.hpp"
MAKE_RAS_MODEL(v2f);
#include "lrr.hpp"
MAKE_RAS_MODEL(LRR);
#include "ssg.hpp"
MAKE_RAS_MODEL(SSG);

// LES models
#include "smagorinsky.hpp"
MAKE_LES_MODEL(Smagorinsky);
#include "wale.hpp"
MAKE_LES_MODEL(WALE);
#include "dynamic_lagrangian.hpp"
MAKE_LES_MODEL(dynamicLagrangian);
#include "k_eqn.hpp"
MAKE_LES_MODEL(kEqn);
#include "dynamic_k_eqn.hpp"
MAKE_LES_MODEL(dynamicKEqn);
#include "spalart_allmaras_des.hpp"
MAKE_LES_MODEL(SpalartAllmarasDES);
#include "spalart_allmaras_ddes.hpp"
MAKE_LES_MODEL(SpalartAllmarasDDES);
#include "spalart_allmaras_iddes.hpp"
MAKE_LES_MODEL(SpalartAllmarasIDDES);
#include "deardorff_diff_stress.hpp"
MAKE_LES_MODEL(DeardorffDiffStress);
