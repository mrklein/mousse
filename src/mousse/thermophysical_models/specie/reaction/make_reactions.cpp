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


#define MAKE_REACTIONS(Thermo, Reaction)                                      \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(Reaction, 0);                           \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(Reaction, Istream);                \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(Reaction, dictionary);             \
                                                                              \
  MAKE_IRN_REACTIONS(Thermo, ArrheniusReactionRate)                           \
  MAKE_IRN_REACTIONS(Thermo, infiniteReactionRate)                            \
  MAKE_IRN_REACTIONS(Thermo, LandauTellerReactionRate)                        \
  MAKE_IRN_REACTIONS(Thermo, thirdBodyArrheniusReactionRate)                  \
                                                                              \
  MAKE_IR_REACTIONS(Thermo, JanevReactionRate)                                \
  MAKE_IR_REACTIONS(Thermo, powerSeriesReactionRate)                          \
                                                                              \
  MAKE_PRESSURE_DEPENDENT_REACTIONS                                           \
  (                                                                           \
   Thermo,                                                                    \
   ArrheniusReactionRate,                                                     \
   LindemannFallOffFunction                                                   \
  )                                                                           \
                                                                              \
  MAKE_PRESSURE_DEPENDENT_REACTIONS                                           \
  (                                                                           \
   Thermo,                                                                    \
   ArrheniusReactionRate,                                                     \
   TroeFallOffFunction                                                        \
  )                                                                           \
                                                                              \
  MAKE_PRESSURE_DEPENDENT_REACTIONS                                           \
  (                                                                           \
   Thermo,                                                                    \
   ArrheniusReactionRate,                                                     \
   SRIFallOffFunction                                                         \
  )


namespace mousse {

// sensible enthalpy based reactions
MAKE_REACTIONS(constGasHThermoPhysics, constGasHReaction)
MAKE_REACTIONS(gasHThermoPhysics, gasHReaction)
MAKE_REACTIONS
(
  constIncompressibleGasHThermoPhysics,
  constIncompressibleGasHReaction
)
MAKE_REACTIONS(incompressibleGasHThermoPhysics, incompressibleGasHReaction)
MAKE_REACTIONS(icoPoly8HThermoPhysics, icoPoly8HReaction)
MAKE_REACTIONS(constGasEThermoPhysics, constGasEReaction)
MAKE_REACTIONS(gasEThermoPhysics, gasEReaction)
MAKE_REACTIONS
(
  constIncompressibleGasEThermoPhysics,
  constIncompressibleGasEReaction
)
MAKE_REACTIONS(incompressibleGasEThermoPhysics, incompressibleGasEReaction)
MAKE_REACTIONS(icoPoly8EThermoPhysics, icoPoly8EReaction)

}

