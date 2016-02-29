#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_PARCEL_FORCES_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_PARCEL_FORCES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "_sphere_drag_force.hpp"
#include "_non_sphere_drag_force.hpp"
#include "_wen_yu_drag_force.hpp"
#include "_ergun_wen_yu_drag_force.hpp"
#include "_plessis_masliyah_drag_force.hpp"
#include "_saffman_mei_lift_force.hpp"
#include "_tomiyama_lift_force.hpp"
#include "_gravity_force.hpp"
#include "_non_inertial_frame_force.hpp"
#include "_paramagnetic_force.hpp"
#include "_pressure_gradient_force.hpp"
#include "_srf_force.hpp"
#include "_virtual_mass_force.hpp"

#define MAKE_PARCEL_FORCES(CloudType)                                         \
                                                                              \
  MAKE_PARTICLE_FORCE_MODEL(CloudType);                                       \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(SphereDragForce, CloudType);                 \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(NonSphereDragForce, CloudType);              \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(WenYuDragForce, CloudType);                  \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(ErgunWenYuDragForce, CloudType);             \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(PlessisMasliyahDragForce, CloudType);        \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(SaffmanMeiLiftForce, CloudType);             \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(TomiyamaLiftForce, CloudType);               \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(GravityForce, CloudType);                    \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(NonInertialFrameForce, CloudType);           \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(ParamagneticForce, CloudType);               \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(PressureGradientForce, CloudType);           \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(SRFForce, CloudType);                        \
  MAKE_PARTICLE_FORCE_MODEL_TYPE(VirtualMassForce, CloudType);

#endif
