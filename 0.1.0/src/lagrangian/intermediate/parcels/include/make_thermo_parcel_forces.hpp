// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_thermo_parcel_forces_hpp_
#define make_thermo_parcel_forces_hpp_

#include "_sphere_drag_force.hpp"
#include "_non_sphere_drag_force.hpp"
#include "_saffman_mei_lift_force.hpp"
#include "_tomiyama_lift_force.hpp"
#include "_gravity_force.hpp"
#include "_non_inertial_frame_force.hpp"
#include "_paramagnetic_force.hpp"
#include "_pressure_gradient_force.hpp"
#include "_srf_force.hpp"
#include "_virtual_mass_force.hpp"

#define makeThermoParcelForces(CloudType)                                     \
                                                                              \
  makeParticleForceModel(CloudType);                                          \
  makeParticleForceModelType(SphereDragForce, CloudType);                     \
  makeParticleForceModelType(NonSphereDragForce, CloudType);                  \
  makeParticleForceModelType(SaffmanMeiLiftForce, CloudType);                 \
  makeParticleForceModelType(TomiyamaLiftForce, CloudType);                   \
  makeParticleForceModelType(GravityForce, CloudType);                        \
  makeParticleForceModelType(NonInertialFrameForce, CloudType);               \
  makeParticleForceModelType(ParamagneticForce, CloudType);                   \
  makeParticleForceModelType(PressureGradientForce, CloudType);               \
  makeParticleForceModelType(SRFForce, CloudType);                            \
  makeParticleForceModelType(VirtualMassForce, CloudType);

#endif
