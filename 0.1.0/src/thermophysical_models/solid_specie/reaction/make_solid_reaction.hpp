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
#define makeSolidReaction(ReactionType, Thermo, ReactionRate)                 \
                                       \
  typedef solidReaction<Thermo> solidReaction##Thermo;                      \
                                       \
  typedef Reaction<Thermo> Reaction##Thermo;                                \
                                       \
  typedef ReactionType<solidReaction, Thermo, ReactionRate>                 \
    ReactionType##Thermo##ReactionRate;                                   \
                                       \
  defineTemplateRunTimeSelectionTable(Reaction##Thermo, Istream);           \
  defineTemplateRunTimeSelectionTable(Reaction##Thermo, dictionary);        \
                                       \
  defineTemplateTypeNameAndDebug(solidReaction##Thermo, 0);                 \
  defineTemplateTypeNameAndDebug(Reaction##Thermo, 0);                      \
                                       \
  template<>                                                                \
  const word ReactionType##Thermo##ReactionRate::typeName                   \
  (                                                                         \
    ReactionType::typeName_()                                             \
   + ReactionRate::type()                                                  \
   + solidReaction##Thermo::typeName_()                                    \
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
#define makeSolidIRReactions(Thermo, ReactionRate)                            \
                                       \
  makeSolidReaction(IrreversibleReaction, Thermo, ReactionRate)
}  // namespace mousse
#endif
