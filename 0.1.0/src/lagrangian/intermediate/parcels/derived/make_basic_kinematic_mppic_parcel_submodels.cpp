// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_kinematic_mppic_cloud.hpp"
#include "make_parcel_cloud_function_objects.hpp"
// Kinematic sub-models
#include "make_parcel_forces.hpp"
#include "make_parcel_dispersion_models.hpp"
#include "make_parcel_injection_models.hpp"
#include "make_parcel_patch_interaction_models.hpp"
#include "make_parcel_stochastic_collision_models.hpp"
#include "make_parcel_surface_film_models.hpp"
// MPPIC sub-models
#include "make_mppic_parcel_damping_models.hpp"
#include "make_mppic_parcel_isotropy_models.hpp"
#include "make_mppic_parcel_packing_models.hpp"
makeParcelCloudFunctionObjects(basicKinematicMPPICCloud);
// Kinematic sub-models
makeParcelForces(basicKinematicMPPICCloud);
makeParcelDispersionModels(basicKinematicMPPICCloud);
makeParcelInjectionModels(basicKinematicMPPICCloud);
makeParcelPatchInteractionModels(basicKinematicMPPICCloud);
makeParcelStochasticCollisionModels(basicKinematicMPPICCloud);
makeParcelSurfaceFilmModels(basicKinematicMPPICCloud);
// MPPIC sub-models
makeMPPICParcelDampingModels(basicKinematicMPPICCloud);
makeMPPICParcelIsotropyModels(basicKinematicMPPICCloud);
makeMPPICParcelPackingModels(basicKinematicMPPICCloud);
