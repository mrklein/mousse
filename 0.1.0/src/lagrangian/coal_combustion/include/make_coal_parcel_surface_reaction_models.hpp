// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_coal_parcel_surface_reaction_models_hpp_
#define make_coal_parcel_surface_reaction_models_hpp_
#include "_no_surface_reaction.hpp"
#include "c_oxidation_diffusion_limited_rate.hpp"
#include "c_oxidation_intrinsic_rate.hpp"
#include "c_oxidation_kinetic_diffusion_limited_rate.hpp"
#include "c_oxidation_hurt_mitchell.hpp"
#include "c_oxidation_murphy_shaddix.hpp"
#define makeCoalParcelSurfaceReactionModels(CloudType)                        \
                                       \
  makeSurfaceReactionModelType(COxidationDiffusionLimitedRate, CloudType);  \
  makeSurfaceReactionModelType                                              \
  (                                                                         \
    COxidationKineticDiffusionLimitedRate,                                \
    CloudType                                                             \
  );                                                                        \
  makeSurfaceReactionModelType(COxidationIntrinsicRate, CloudType);         \
  makeSurfaceReactionModelType(COxidationHurtMitchell, CloudType);          \
  makeSurfaceReactionModelType(COxidationMurphyShaddix, CloudType);
#endif
