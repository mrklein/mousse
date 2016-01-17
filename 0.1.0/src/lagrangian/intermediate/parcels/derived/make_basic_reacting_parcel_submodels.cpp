// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_reacting_cloud.hpp"
#include "make_parcel_cloud_function_objects.hpp"
// Kinematic
#include "make_thermo_parcel_forces.hpp" // thermo variant
#include "make_parcel_dispersion_models.hpp"
#include "make_reacting_parcel_injection_models.hpp" // Reacting variant
#include "make_parcel_patch_interaction_models.hpp"
#include "make_parcel_stochastic_collision_models.hpp"
#include "make_reacting_parcel_surface_film_models.hpp" // Reacting variant
// Thermodynamic
#include "make_parcel_heat_transfer_models.hpp"
// Reacting
#include "make_reacting_parcel_composition_models.hpp"
#include "make_reacting_parcel_phase_change_models.hpp"
MAKE_PARCEL_CLOUD_FUNCTION_OBJECTS(basicReactingCloud);
// Kinematic sub-models
MAKE_THERMO_PARCEL_FORCES(basicReactingCloud);
MAKE_PARCEL_DISPERSION_MODELS(basicReactingCloud);
MAKE_REACTING_PARCEL_INJECTION_MODELS(basicReactingCloud);
MAKE_PARCEL_PATCH_INTERACTION_MODELS(basicReactingCloud);
MAKE_PARCEL_STOCHASTIC_COLLISION_MODELS(basicReactingCloud);
MAKE_REACTING_PARCEL_SURFACE_FILM_MODELS(basicReactingCloud);
// Thermo sub-models
MAKE_PARCEL_HEAT_TRANSFER_MODELS(basicReactingCloud);
// Reacting sub-models
MAKE_REACTING_PARCEL_COMPOSITION_MODELS(basicReactingCloud);
MAKE_REACTING_PARCEL_PHASE_CHANGE_MODELS(basicReactingCloud);
