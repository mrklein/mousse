// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_reacting_multiphase_cloud.hpp"
#include "make_parcel_cloud_function_objects.hpp"
// Kinematic
#include "make_thermo_parcel_forces.hpp" // thermo variant
#include "make_parcel_dispersion_models.hpp"
#include "make_reacting_multiphase_parcel_injection_models.hpp" // MP variant
#include "make_parcel_patch_interaction_models.hpp"
#include "make_reacting_multiphase_parcel_stochastic_collision_models.hpp" // MP variant
#include "make_reacting_parcel_surface_film_models.hpp" // Reacting variant
// Thermodynamic
#include "make_parcel_heat_transfer_models.hpp"
// Reacting
#include "make_reacting_multiphase_parcel_composition_models.hpp" // MP Variant
#include "make_reacting_parcel_phase_change_models.hpp"
// Reacting multiphase
#include "make_reacting_multiphase_parcel_devolatilisation_models.hpp"
#include "make_reacting_multiphase_parcel_surface_reaction_models.hpp"
makeParcelCloudFunctionObjects(basicReactingMultiphaseCloud);
// Kinematic sub-models
makeThermoParcelForces(basicReactingMultiphaseCloud);
makeParcelDispersionModels(basicReactingMultiphaseCloud);
makeReactingMultiphaseParcelInjectionModels(basicReactingMultiphaseCloud);
makeParcelPatchInteractionModels(basicReactingMultiphaseCloud);
makeReactingMultiphaseParcelStochasticCollisionModels
(
  basicReactingMultiphaseCloud
);
makeReactingParcelSurfaceFilmModels(basicReactingMultiphaseCloud);
// Thermo sub-models
makeParcelHeatTransferModels(basicReactingMultiphaseCloud);
// Reacting sub-models
makeReactingMultiphaseParcelCompositionModels
(
  basicReactingMultiphaseCloud
);
makeReactingParcelPhaseChangeModels(basicReactingMultiphaseCloud);
// Reacting multiphase sub-models
makeReactingMultiphaseParcelDevolatilisationModels
(
  basicReactingMultiphaseCloud
);
makeReactingMultiphaseParcelSurfaceReactionModels
(
  basicReactingMultiphaseCloud
);
