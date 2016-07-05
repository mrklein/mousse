// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reversible_reaction.hpp"


// Constructors 
template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::ReversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
ReversibleReaction
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
mousse::ReversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
ReversibleReaction
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
mousse::ReversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
ReversibleReaction
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
mousse::ReversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
ReversibleReaction
(
  const ReversibleReaction<ReactionType, ReactionThermo, ReactionRate>& rr,
  const speciesTable& species
)
:
  ReactionType<ReactionThermo>{rr, species},
  k_{rr.k_}
{}


// Member Functions 
template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::scalar mousse::ReversibleReaction
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
mousse::scalar mousse::ReversibleReaction
<
  ReactionType,
  ReactionThermo,
  ReactionRate
>::kr
(
  const scalar kfwd,
  const scalar p,
  const scalar T,
  const scalarField& /*c*/
) const
{
  scalar Kc = this->Kc(p, T);
  if (mag(Kc) > VSMALL) {
    return kfwd/Kc;
  } else {
    return 0;
  }
}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::scalar mousse::ReversibleReaction
<
  ReactionType,
  ReactionThermo,
  ReactionRate
>::kr
(
  const scalar p,
  const scalar T,
  const scalarField& c
) const
{
  return kr(kf(p, T, c), p, T, c);
}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
void mousse::ReversibleReaction
<
  ReactionType,
  ReactionThermo,
  ReactionRate
>::write
(
  Ostream& os
) const
{
  Reaction<ReactionThermo>::write(os);
  k_.write(os);
}

