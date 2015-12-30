// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_kinematic_cloud.hpp"
#include "make_parcel_cloud_function_objects.hpp"
// Kinematic
#include "make_parcel_forces.hpp"
#include "make_parcel_dispersion_models.hpp"
#include "make_parcel_injection_models.hpp"
#include "make_parcel_patch_interaction_models.hpp"
#include "make_parcel_stochastic_collision_models.hpp"
#include "make_parcel_surface_film_models.hpp"
makeParcelCloudFunctionObjects(basicKinematicCloud);
// Kinematic sub-models
makeParcelForces(basicKinematicCloud);
makeParcelDispersionModels(basicKinematicCloud);
makeParcelInjectionModels(basicKinematicCloud);
makeParcelPatchInteractionModels(basicKinematicCloud);
makeParcelStochasticCollisionModels(basicKinematicCloud);
makeParcelSurfaceFilmModels(basicKinematicCloud);
