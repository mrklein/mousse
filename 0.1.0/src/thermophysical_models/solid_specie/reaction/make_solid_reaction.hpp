// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef makeSolidReactionThermo_H
#define makeSolidReactionThermo_H
#include "solid_reaction.hpp"
#include "irreversible_reaction.hpp"
#include "reaction.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
#define MAKE_SOLID_REACTION(ReactionType, Thermo, ReactionRate)               \
                                                                              \
  typedef solidReaction<Thermo> solidReaction##Thermo;                        \
                                                                              \
  typedef Reaction<Thermo> Reaction##Thermo;                                  \
                                                                              \
  typedef ReactionType<solidReaction, Thermo, ReactionRate>                   \
    ReactionType##Thermo##ReactionRate;                                       \
                                                                              \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(Reaction##Thermo, Istream);        \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(Reaction##Thermo, dictionary);     \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(solidReaction##Thermo, 0);              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(Reaction##Thermo, 0);                   \
                                                                              \
  template<>                                                                  \
  const word ReactionType##Thermo##ReactionRate::typeName                     \
  (                                                                           \
    ReactionType::typeName_()                                                 \
   + ReactionRate::type()                                                     \
   + solidReaction##Thermo::typeName_()                                       \
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


#define MAKE_SOLID_IR_REACTIONS(Thermo, ReactionRate)                         \
                                                                              \
  MAKE_SOLID_REACTION(IrreversibleReaction, Thermo, ReactionRate)
}  // namespace mousse
#endif
