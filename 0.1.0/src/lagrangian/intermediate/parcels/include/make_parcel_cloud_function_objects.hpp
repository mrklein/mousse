// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_cloud_function_objects_hpp_
#define make_parcel_cloud_function_objects_hpp_

#include "_face_post_processing.hpp"
#include "_particle_collector.hpp"
#include "_particle_erosion.hpp"
#include "_particle_tracks.hpp"
#include "_particle_trap.hpp"
#include "_patch_post_processing.hpp"
#include "_void_fraction.hpp"

#define makeParcelCloudFunctionObjects(CloudType)                             \
                                                                              \
  makeCloudFunctionObject(CloudType);                                         \
                                                                              \
  makeCloudFunctionObjectType(FacePostProcessing, CloudType);                 \
  makeCloudFunctionObjectType(ParticleCollector, CloudType);                  \
  makeCloudFunctionObjectType(ParticleErosion, CloudType);                    \
  makeCloudFunctionObjectType(ParticleTracks, CloudType);                     \
  makeCloudFunctionObjectType(ParticleTrap, CloudType);                       \
  makeCloudFunctionObjectType(PatchPostProcessing, CloudType);                \
  makeCloudFunctionObjectType(VoidFraction, CloudType);

#endif
