// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "non_equilibrium_reversible_reaction.hpp"


// Constructors 
template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::NonEquilibriumReversibleReaction
<
  ReactionType,
  ReactionThermo,
  ReactionRate
>::
NonEquilibriumReversibleReaction
(
  const ReactionType<ReactionThermo>& reaction,
  const ReactionRate& forwardReactionRate,
  const ReactionRate& reverseReactionRate
)
:
  ReactionType<ReactionThermo>{reaction},
  fk_{forwardReactionRate},
  rk_{reverseReactionRate}
{}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::NonEquilibriumReversibleReaction
<
  ReactionType,
  ReactionThermo,
  ReactionRate
>::
NonEquilibriumReversibleReaction
(
  const speciesTable& species,
  const HashPtrTable<ReactionThermo>& thermoDatabase,
  Istream& is
)
:
  ReactionType<ReactionThermo>{species, thermoDatabase, is},
  fk_{species, is},
  rk_{species, is}
{}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::NonEquilibriumReversibleReaction
<
  ReactionType,
  ReactionThermo,
  ReactionRate
>::
NonEquilibriumReversibleReaction
(
  const speciesTable& species,
  const HashPtrTable<ReactionThermo>& thermoDatabase,
  const dictionary& dict
)
:
  ReactionType<ReactionThermo>{species, thermoDatabase, dict},
  fk_{species, dict.subDict("forward")},
  rk_{species, dict.subDict("reverse")}
{}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::NonEquilibriumReversibleReaction
<
  ReactionType,
  ReactionThermo,
  ReactionRate
>::
NonEquilibriumReversibleReaction
(
  const NonEquilibriumReversibleReaction
  <
    ReactionType,
    ReactionThermo,
    ReactionRate
  >& nerr,
  const speciesTable& species
)
:
  ReactionType<ReactionThermo>{nerr, species},
  fk_{nerr.fk_},
  rk_{nerr.rk_}
{}


// Member Functions 
template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::scalar
mousse::NonEquilibriumReversibleReaction
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
  return fk_(p, T, c);
}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::scalar
mousse::NonEquilibriumReversibleReaction
<
  ReactionType,
  ReactionThermo,
  ReactionRate
>::kr
(
  const scalar,
  const scalar p,
  const scalar T,
  const scalarField& c
) const
{
  return rk_(p, T, c);
}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
mousse::scalar
mousse::NonEquilibriumReversibleReaction
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
  return rk_(p, T, c);
}


template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
void mousse::NonEquilibriumReversibleReaction
<
  ReactionType,
  ReactionThermo,
  ReactionRate
>::write
(
  Ostream& os
) const
{
  ReactionType<ReactionThermo>::write(os);
  os  << indent << "forward" << nl;
  os  << indent << token::BEGIN_BLOCK << nl;
  os  << incrIndent;
  fk_.write(os);
  os  << decrIndent;
  os  << indent << token::END_BLOCK << nl;
  os  << indent << "reverse" << nl;
  os  << indent << token::BEGIN_BLOCK << nl;
  os  << incrIndent;
  rk_.write(os);
  os  << decrIndent;
  os  << indent << token::END_BLOCK << nl;
}

