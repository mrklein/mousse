// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_thermo_parcel_turbulence_forces_hpp_
#define make_thermo_parcel_turbulence_forces_hpp_

#include "_brownian_motion_force.hpp"

#define makeThermoParcelTurbulenceForces(CloudType)                           \
                                                                              \
  makeParticleForceModelType(BrownianMotionForce, CloudType);

#endif
