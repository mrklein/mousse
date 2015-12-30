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
makeParcelCloudFunctionObjects(basicThermoCloud);
// Kinematic sub-models
makeThermoParcelForces(basicThermoCloud);
makeParcelDispersionModels(basicThermoCloud);
makeParcelInjectionModels(basicThermoCloud);
makeParcelPatchInteractionModels(basicThermoCloud);
makeParcelStochasticCollisionModels(basicThermoCloud);
makeParcelSurfaceFilmModels(basicThermoCloud);
// Thermo sub-models
makeParcelHeatTransferModels(basicThermoCloud);
