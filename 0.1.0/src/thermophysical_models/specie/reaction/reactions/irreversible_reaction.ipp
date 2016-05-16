// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "irreversible_reaction.hpp"


// Constructors 
template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::IrreversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
IrreversibleReaction
(
  const ReactionType<ReactionThermo>& reaction,
  const ReactionRate& k
)
:
  ReactionType<ReactionThermo>{reaction},
  k_{k}
{}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::IrreversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
IrreversibleReaction
(
  const speciesTable& species,
  const HashPtrTable<ReactionThermo>& thermoDatabase,
  Istream& is
)
:
  ReactionType<ReactionThermo>{species, thermoDatabase, is},
  k_{species, is}
{}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::IrreversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
IrreversibleReaction
(
  const speciesTable& species,
  const HashPtrTable<ReactionThermo>& thermoDatabase,
  const dictionary& dict
)
:
  ReactionType<ReactionThermo>{species, thermoDatabase, dict},
  k_{species, dict}
{}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::IrreversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
IrreversibleReaction
(
  const IrreversibleReaction<ReactionType, ReactionThermo,ReactionRate>& irr,
  const speciesTable& species
)
:
  ReactionType<ReactionThermo>{irr, species},
  k_{irr.k_}
{}


// Member Functions 
template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::scalar mousse::IrreversibleReaction
<
  ReactionType,
  ReactionThermo,
  ReactionRate
>::kf
(
  const scalar p,
  const scalar T,
  const scalarField& c
) const
{
  return k_(p, T, c);
}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
void mousse::IrreversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
write
(
  Ostream& os
) const
{
  ReactionType<ReactionThermo>::write(os);
  k_.write(os);
}

