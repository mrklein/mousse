// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reaction_hpp_
#define make_reaction_hpp_
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
#define makeReaction(Thermo, ReactionType, ReactionRate)                      \
                                       \
  typedef Reaction<Thermo> Reaction##Thermo;                                \
                                       \
  typedef ReactionType<Reaction, Thermo, ReactionRate>                      \
    ReactionType##Thermo##ReactionRate;                                   \
                                       \
  template<>                                                                \
  const word ReactionType##Thermo##ReactionRate::typeName                   \
  (                                                                         \
    ReactionType::typeName_()                                             \
   + ReactionRate::type()                                                  \
   + Reaction##Thermo::typeName_()                                         \
  );                                                                        \
                                       \
  addToRunTimeSelectionTable                                                \
  (                                                                         \
    Reaction##Thermo,                                                     \
    ReactionType##Thermo##ReactionRate,                                   \
    Istream                                                               \
  );                                                                        \
                                       \
  addToRunTimeSelectionTable                                                \
  (                                                                         \
    Reaction##Thermo,                                                     \
    ReactionType##Thermo##ReactionRate,                                   \
    dictionary                                                            \
  );
#define makePressureDependentReaction\
(                                                                             \
  Thermo,                                                                   \
  Reaction,                                                                 \
  PressureDependentReactionRate,                                            \
  ReactionRate,                                                             \
  FallOffFunction                                                           \
)                                                                             \
                                       \
  typedef PressureDependentReactionRate<ReactionRate, FallOffFunction>      \
    PressureDependentReactionRate##ReactionRate##FallOffFunction;         \
                                       \
  makeReaction                                                              \
  (                                                                         \
    Thermo,                                                               \
    Reaction,                                                             \
    PressureDependentReactionRate##ReactionRate##FallOffFunction          \
  )
#define makeIRReactions(Thermo, ReactionRate)                                 \
                                       \
  makeReaction(Thermo, IrreversibleReaction, ReactionRate)                  \
                                       \
  makeReaction(Thermo, ReversibleReaction, ReactionRate)
#define makeIRNReactions(Thermo, ReactionRate)                                \
                                       \
  makeIRReactions(Thermo, ReactionRate)                                     \
                                       \
  makeReaction(Thermo, NonEquilibriumReversibleReaction, ReactionRate)
#define makePressureDependentReactions(Thermo, ReactionRate, FallOffFunction) \
                                       \
  makePressureDependentReaction                                             \
  (                                                                         \
    Thermo,                                                               \
    IrreversibleReaction,                                                 \
    FallOffReactionRate,                                                  \
    ReactionRate,                                                         \
    FallOffFunction                                                       \
  )                                                                         \
                                       \
  makePressureDependentReaction                                             \
  (                                                                         \
    Thermo,                                                               \
    ReversibleReaction,                                                   \
    FallOffReactionRate,                                                  \
    ReactionRate,                                                         \
    FallOffFunction                                                       \
  )                                                                         \
                                       \
  makePressureDependentReaction                                             \
  (                                                                         \
    Thermo,                                                               \
    IrreversibleReaction,                                                 \
    ChemicallyActivatedReactionRate,                                      \
    ReactionRate,                                                         \
    FallOffFunction                                                       \
  )                                                                         \
                                       \
  makePressureDependentReaction                                             \
  (                                                                         \
    Thermo,                                                               \
    ReversibleReaction,                                                   \
    ChemicallyActivatedReactionRate,                                      \
    ReactionRate,                                                         \
    FallOffFunction                                                       \
  )
}  // namespace mousse
#endif
