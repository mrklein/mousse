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
makeBaseTurbulenceModel
(
  geometricOneField,
  geometricOneField,
  incompressibleTurbulenceModel,
  IncompressibleTurbulenceModel,
  transportModel
);

#define makeRASModel(Type)                                                    \
  makeTemplatedTurbulenceModel                                                \
  (transportModelIncompressibleTurbulenceModel, RAS, Type)

#define makeLESModel(Type)                                                    \
  makeTemplatedTurbulenceModel                                                \
  (transportModelIncompressibleTurbulenceModel, LES, Type)

// -------------------------------------------------------------------------- //
// RAS models
// -------------------------------------------------------------------------- //
#include "spalart_allmaras.hpp"
makeRASModel(SpalartAllmaras);
#include "k_epsilon.hpp"
makeRASModel(kEpsilon);
#include "rng_k_epsilon.hpp"
makeRASModel(RNGkEpsilon);
#include "realizable_ke.hpp"
makeRASModel(realizableKE);
#include "launder_sharma_ke.hpp"
makeRASModel(LaunderSharmaKE);
#include "k_omega.hpp"
makeRASModel(kOmega);
#include "k_omega_sst.hpp"
makeRASModel(kOmegaSST);
#include "k_omega_sstsas.hpp"
makeRASModel(kOmegaSSTSAS);
#include "v2f.hpp"
makeRASModel(v2f);
#include "lrr.hpp"
makeRASModel(LRR);
#include "ssg.hpp"
makeRASModel(SSG);
// -------------------------------------------------------------------------- //
// LES models
// -------------------------------------------------------------------------- //
#include "smagorinsky.hpp"
makeLESModel(Smagorinsky);
#include "wale.hpp"
makeLESModel(WALE);
#include "dynamic_lagrangian.hpp"
makeLESModel(dynamicLagrangian);
#include "k_eqn.hpp"
makeLESModel(kEqn);
#include "dynamic_k_eqn.hpp"
makeLESModel(dynamicKEqn);
#include "spalart_allmaras_des.hpp"
makeLESModel(SpalartAllmarasDES);
#include "spalart_allmaras_ddes.hpp"
makeLESModel(SpalartAllmarasDDES);
#include "spalart_allmaras_iddes.hpp"
makeLESModel(SpalartAllmarasIDDES);
#include "deardorff_diff_stress.hpp"
makeLESModel(DeardorffDiffStress);
