#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_MAKE_REACTION_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_MAKE_REACTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reaction.hpp"
#include "irreversible_reaction.hpp"
#include "reversible_reaction.hpp"
#include "non_equilibrium_reversible_reaction.hpp"
#include "thermo.hpp"
#include "sutherland_transport.hpp"
#include "janaf_thermo.hpp"
#include "perfect_gas.hpp"
#include "polynomial_transport.hpp"
#include "h_polynomial_thermo.hpp"
#include "ico_polynomial.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
#define MAKE_REACTION(Thermo, ReactionType, ReactionRate)                     \
                                                                              \
  typedef Reaction<Thermo> Reaction##Thermo;                                  \
                                                                              \
  typedef ReactionType<Reaction, Thermo, ReactionRate>                        \
    ReactionType##Thermo##ReactionRate;                                       \
                                                                              \
  template<>                                                                  \
  const word ReactionType##Thermo##ReactionRate::typeName                     \
  (                                                                           \
    ReactionType::typeName_()                                                 \
   + ReactionRate::type()                                                     \
   + Reaction##Thermo::typeName_()                                            \
  );                                                                          \
                                                                              \
  ADD_TO_RUN_TIME_SELECTION_TABLE                                             \
  (                                                                           \
    Reaction##Thermo,                                                         \
    ReactionType##Thermo##ReactionRate,                                       \
    Istream                                                                   \
  );                                                                          \
                                                                              \
  ADD_TO_RUN_TIME_SELECTION_TABLE                                             \
  (                                                                           \
    Reaction##Thermo,                                                         \
    ReactionType##Thermo##ReactionRate,                                       \
    dictionary                                                                \
  );


#define MAKE_PRESSURE_DEPENDENT_REACTION\
(                                                                             \
  Thermo,                                                                     \
  Reaction,                                                                   \
  PressureDependentReactionRate,                                              \
  ReactionRate,                                                               \
  FallOffFunction                                                             \
)                                                                             \
                                                                              \
  typedef PressureDependentReactionRate<ReactionRate, FallOffFunction>        \
    PressureDependentReactionRate##ReactionRate##FallOffFunction;             \
                                                                              \
  MAKE_REACTION                                                               \
  (                                                                           \
    Thermo,                                                                   \
    Reaction,                                                                 \
    PressureDependentReactionRate##ReactionRate##FallOffFunction              \
  )


#define MAKE_IR_REACTIONS(Thermo, ReactionRate)                               \
                                                                              \
  MAKE_REACTION(Thermo, IrreversibleReaction, ReactionRate)                   \
                                                                              \
  MAKE_REACTION(Thermo, ReversibleReaction, ReactionRate)

#define MAKE_IRN_REACTIONS(Thermo, ReactionRate)                              \
                                                                              \
  MAKE_IR_REACTIONS(Thermo, ReactionRate)                                     \
                                                                              \
  MAKE_REACTION(Thermo, NonEquilibriumReversibleReaction, ReactionRate)


#define MAKE_PRESSURE_DEPENDENT_REACTIONS(Thermo, ReactionRate, FallOffFunction)\
                                                                              \
  MAKE_PRESSURE_DEPENDENT_REACTION                                            \
  (                                                                           \
    Thermo,                                                                   \
    IrreversibleReaction,                                                     \
    FallOffReactionRate,                                                      \
    ReactionRate,                                                             \
    FallOffFunction                                                           \
  )                                                                           \
                                                                              \
  MAKE_PRESSURE_DEPENDENT_REACTION                                            \
  (                                                                           \
    Thermo,                                                                   \
    ReversibleReaction,                                                       \
    FallOffReactionRate,                                                      \
    ReactionRate,                                                             \
    FallOffFunction                                                           \
  )                                                                           \
                                                                              \
  MAKE_PRESSURE_DEPENDENT_REACTION                                            \
  (                                                                           \
    Thermo,                                                                   \
    IrreversibleReaction,                                                     \
    ChemicallyActivatedReactionRate,                                          \
    ReactionRate,                                                             \
    FallOffFunction                                                           \
  )                                                                           \
                                                                              \
  MAKE_PRESSURE_DEPENDENT_REACTION                                            \
  (                                                                           \
    Thermo,                                                                   \
    ReversibleReaction,                                                       \
    ChemicallyActivatedReactionRate,                                          \
    ReactionRate,                                                             \
    FallOffFunction                                                           \
  )
}  // namespace mousse
#endif
