#ifndef LAGRANGIAN_COAL_COMBUSTION_INCLUDE_MAKE_COAL_PARCEL_SURFACE_REACTION_MODELS_HPP_
#define LAGRANGIAN_COAL_COMBUSTION_INCLUDE_MAKE_COAL_PARCEL_SURFACE_REACTION_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_surface_reaction.hpp"
#include "c_oxidation_diffusion_limited_rate.hpp"
#include "c_oxidation_intrinsic_rate.hpp"
#include "c_oxidation_kinetic_diffusion_limited_rate.hpp"
#include "c_oxidation_hurt_mitchell.hpp"
#include "c_oxidation_murphy_shaddix.hpp"
#define MAKE_COAL_PARCEL_SURFACE_REACTION_MODELS(CloudType)                   \
                                                                              \
  MAKE_SURFACE_REACTION_MODEL_TYPE(COxidationDiffusionLimitedRate, CloudType);\
  MAKE_SURFACE_REACTION_MODEL_TYPE                                            \
  (                                                                           \
    COxidationKineticDiffusionLimitedRate,                                    \
    CloudType                                                                 \
  );                                                                          \
  MAKE_SURFACE_REACTION_MODEL_TYPE(COxidationIntrinsicRate, CloudType);       \
  MAKE_SURFACE_REACTION_MODEL_TYPE(COxidationHurtMitchell, CloudType);        \
  MAKE_SURFACE_REACTION_MODEL_TYPE(COxidationMurphyShaddix, CloudType);
#endif
