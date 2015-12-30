// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reaction_types.hpp"
#include "make_reaction.hpp"
#include "arrhenius_reaction_rate.hpp"
#include "infinite_reaction_rate.hpp"
#include "landau_teller_reaction_rate.hpp"
#include "third_body_arrhenius_reaction_rate.hpp"
#include "chemically_activated_reaction_rate.hpp"
#include "janev_reaction_rate.hpp"
#include "power_series_reaction_rate.hpp"
#include "fall_off_reaction_rate.hpp"
#include "lindemann_fall_off_function.hpp"
#include "sri_fall_off_function.hpp"
#include "troe_fall_off_function.hpp"
#define makeReactions(Thermo, Reaction)                                        \
                                       \
  defineTemplateTypeNameAndDebug(Reaction, 0);                               \
  defineTemplateRunTimeSelectionTable(Reaction, Istream);                    \
  defineTemplateRunTimeSelectionTable(Reaction, dictionary);                 \
                                       \
  makeIRNReactions(Thermo, ArrheniusReactionRate)                            \
  makeIRNReactions(Thermo, infiniteReactionRate)                             \
  makeIRNReactions(Thermo, LandauTellerReactionRate)                         \
  makeIRNReactions(Thermo, thirdBodyArrheniusReactionRate)                   \
                                       \
  makeIRReactions(Thermo, JanevReactionRate)                                 \
  makeIRReactions(Thermo, powerSeriesReactionRate)                           \
                                       \
  makePressureDependentReactions                                             \
  (                                                                          \
   Thermo,                                                                 \
   ArrheniusReactionRate,                                                  \
   LindemannFallOffFunction                                                \
  )                                                                          \
                                       \
  makePressureDependentReactions                                             \
  (                                                                          \
   Thermo,                                                                 \
   ArrheniusReactionRate,                                                  \
   TroeFallOffFunction                                                     \
  )                                                                          \
                                       \
  makePressureDependentReactions                                             \
  (                                                                          \
   Thermo,                                                                 \
   ArrheniusReactionRate,                                                  \
   SRIFallOffFunction                                                      \
  )
namespace mousse
{
  // sensible enthalpy based reactions
  makeReactions(constGasHThermoPhysics, constGasHReaction)
  makeReactions(gasHThermoPhysics, gasHReaction)
  makeReactions
  (
    constIncompressibleGasHThermoPhysics,
    constIncompressibleGasHReaction
  )
  makeReactions(incompressibleGasHThermoPhysics, incompressibleGasHReaction)
  makeReactions(icoPoly8HThermoPhysics, icoPoly8HReaction)
  makeReactions(constGasEThermoPhysics, constGasEReaction)
  makeReactions(gasEThermoPhysics, gasEReaction)
  makeReactions
  (
    constIncompressibleGasEThermoPhysics,
    constIncompressibleGasEReaction
  )
  makeReactions(incompressibleGasEThermoPhysics, incompressibleGasEReaction)
  makeReactions(icoPoly8EThermoPhysics, icoPoly8EReaction)
}
