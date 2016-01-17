// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_spray_cloud.hpp"
#include "make_parcel_cloud_function_objects.hpp"
// Kinematic
#include "make_thermo_parcel_forces.hpp" // thermo variant
#include "make_thermo_parcel_turbulence_forces.hpp" // add turbulence variant
#include "make_parcel_dispersion_models.hpp"
#include "make_parcel_turbulence_dispersion_models.hpp" // add turbulence variant
#include "make_spray_parcel_injection_models.hpp" // Spray variant
#include "make_parcel_patch_interaction_models.hpp"
#include "make_spray_parcel_stochastic_collision_models.hpp" // Spray variant
// Thermodynamic
#include "make_parcel_heat_transfer_models.hpp"
// Reacting
#include "make_reacting_parcel_composition_models.hpp"
#include "make_reacting_parcel_phase_change_models.hpp"
#include "make_reacting_parcel_surface_film_models.hpp"
// Spray
#include "_distorted_sphere_drag_force.hpp"
#include "make_spray_parcel_atomization_models.hpp"
#include "make_spray_parcel_breakup_models.hpp"
MAKE_PARCEL_CLOUD_FUNCTION_OBJECTS(basicSprayCloud);
// Kinematic sub-models
MAKE_THERMO_PARCEL_FORCES(basicSprayCloud);
MAKE_THERMO_PARCEL_TURBULENCE_FORCES(basicSprayCloud);
MAKE_PARCEL_DISPERSION_MODELS(basicSprayCloud);
MAKE_PARCEL_TURBULENCE_DISPERSION_MODELS(basicSprayCloud);
MAKE_SPRAY_PARCEL_INJECTION_MODELS(basicSprayCloud);
MAKE_PARCEL_PATCH_INTERACTION_MODELS(basicSprayCloud);
MAKE_SPRAY_PARCEL_STOCHASTIC_COLLISION_MODELS(basicSprayCloud);
// Thermo sub-models
MAKE_PARCEL_HEAT_TRANSFER_MODELS(basicSprayCloud);
// Reacting sub-models
MAKE_REACTING_PARCEL_COMPOSITION_MODELS(basicSprayCloud);
MAKE_REACTING_PARCEL_PHASE_CHANGE_MODELS(basicSprayCloud);
MAKE_REACTING_PARCEL_SURFACE_FILM_MODELS(basicSprayCloud);
// Spray sub-models
MAKE_PARTICLE_FORCE_MODEL_TYPE(DistortedSphereDragForce, basicSprayCloud);
MAKE_SPRAY_PARCEL_ATOMIZATION_MODELS(basicSprayCloud);
MAKE_SPRAY_PARCEL_BREAKUP_MODELS(basicSprayCloud);
