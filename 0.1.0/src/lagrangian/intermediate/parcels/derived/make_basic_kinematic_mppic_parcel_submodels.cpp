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
MAKE_PARCEL_CLOUD_FUNCTION_OBJECTS(basicKinematicMPPICCloud);
// Kinematic sub-models
MAKE_PARCEL_FORCES(basicKinematicMPPICCloud);
MAKE_PARCEL_DISPERSION_MODELS(basicKinematicMPPICCloud);
MAKE_PARCEL_INJECTION_MODELS(basicKinematicMPPICCloud);
MAKE_PARCEL_PATCH_INTERACTION_MODELS(basicKinematicMPPICCloud);
MAKE_PARCEL_STOCHASTIC_COLLISION_MODELS(basicKinematicMPPICCloud);
MAKE_PARCEL_SURFACE_FILM_MODELS(basicKinematicMPPICCloud);
// MPPIC sub-models
MAKE_MPPIC_PARCEL_DAMPING_MODELS(basicKinematicMPPICCloud);
MAKE_MPPIC_PARCEL_ISOTROPY_MODELS(basicKinematicMPPICCloud);
MAKE_MPPIC_PARCEL_PACKING_MODELS(basicKinematicMPPICCloud);
