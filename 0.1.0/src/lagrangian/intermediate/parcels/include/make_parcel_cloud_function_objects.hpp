#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_PARCEL_CLOUD_FUNCTION_OBJECTS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_PARCEL_CLOUD_FUNCTION_OBJECTS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "_face_post_processing.hpp"
#include "_particle_collector.hpp"
#include "_particle_erosion.hpp"
#include "_particle_tracks.hpp"
#include "_particle_trap.hpp"
#include "_patch_post_processing.hpp"
#include "_void_fraction.hpp"

#define MAKE_PARCEL_CLOUD_FUNCTION_OBJECTS(CloudType)                         \
                                                                              \
  MAKE_CLOUD_FUNCTION_OBJECT(CloudType);                                      \
                                                                              \
  MAKE_CLOUD_FUNCTION_OBJECT_TYPE(FacePostProcessing, CloudType);             \
  MAKE_CLOUD_FUNCTION_OBJECT_TYPE(ParticleCollector, CloudType);              \
  MAKE_CLOUD_FUNCTION_OBJECT_TYPE(ParticleErosion, CloudType);                \
  MAKE_CLOUD_FUNCTION_OBJECT_TYPE(ParticleTracks, CloudType);                 \
  MAKE_CLOUD_FUNCTION_OBJECT_TYPE(ParticleTrap, CloudType);                   \
  MAKE_CLOUD_FUNCTION_OBJECT_TYPE(PatchPostProcessing, CloudType);            \
  MAKE_CLOUD_FUNCTION_OBJECT_TYPE(VoidFraction, CloudType);

#endif
