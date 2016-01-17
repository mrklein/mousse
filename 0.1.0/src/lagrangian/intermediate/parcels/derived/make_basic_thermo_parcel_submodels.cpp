// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_thermo_cloud.hpp"
#include "make_parcel_cloud_function_objects.hpp"
// Kinematic
#include "make_thermo_parcel_forces.hpp" // thermo variant
#include "make_parcel_dispersion_models.hpp"
#include "make_parcel_injection_models.hpp"
#include "make_parcel_patch_interaction_models.hpp"
#include "make_parcel_stochastic_collision_models.hpp"
#include "make_thermo_parcel_surface_film_models.hpp" // thermo variant
// Thermodynamic
#include "make_parcel_heat_transfer_models.hpp"
MAKE_PARCEL_CLOUD_FUNCTION_OBJECTS(basicThermoCloud);
// Kinematic sub-models
MAKE_THERMO_PARCEL_FORCES(basicThermoCloud);
MAKE_PARCEL_DISPERSION_MODELS(basicThermoCloud);
MAKE_PARCEL_INJECTION_MODELS(basicThermoCloud);
MAKE_PARCEL_PATCH_INTERACTION_MODELS(basicThermoCloud);
MAKE_PARCEL_STOCHASTIC_COLLISION_MODELS(basicThermoCloud);
MAKE_PARCEL_SURFACE_FILM_MODELS(basicThermoCloud);
// Thermo sub-models
MAKE_PARCEL_HEAT_TRANSFER_MODELS(basicThermoCloud);
