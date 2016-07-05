#ifndef LAGRANGIAN_TURBULENCE_PARCELS_INCLUDE_MAKE_THERMO_PARCEL_TURBULENCE_FORCES_HPP_
#define LAGRANGIAN_TURBULENCE_PARCELS_INCLUDE_MAKE_THERMO_PARCEL_TURBULENCE_FORCES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_brownian_motion_force.hpp"


#define MAKE_THERMO_PARCEL_TURBULENCE_FORCES(CloudType)                       \
                                                                              \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(BrownianMotionForce, CloudType);

#endif
