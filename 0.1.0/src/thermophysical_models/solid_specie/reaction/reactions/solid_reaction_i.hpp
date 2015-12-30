// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_reaction.hpp"
namespace mousse
{
// Ostream Operator 
template<class ReactionThermo>
inline Ostream& operator<<
(
  Ostream& os,
  const solidReaction<ReactionThermo>& r
)
{
  OStringStream reaction;
  os << r.solidReactionStr(reaction)<< token::END_STATEMENT <<nl;
  return os;
}
}  // namespace mousse
