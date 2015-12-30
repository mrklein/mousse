// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reaction.hpp"
namespace mousse
{
// Member Functions 
template<class ReactionThermo>
inline word& Reaction<ReactionThermo>::name()
{
  return name_;
}
template<class ReactionThermo>
inline const word& Reaction<ReactionThermo>::name() const
{
  return name_;
}
template<class ReactionThermo>
inline const List<typename Reaction<ReactionThermo>::specieCoeffs>&
Reaction<ReactionThermo>::lhs() const
{
  return lhs_;
}
template<class ReactionThermo>
inline const List<typename Reaction<ReactionThermo>::specieCoeffs>&
Reaction<ReactionThermo>::rhs() const
{
  return rhs_;
}
// Ostream Operator 
template<class ReactionThermo>
inline Ostream& operator<<(Ostream& os, const Reaction<ReactionThermo>& r)
{
  OStringStream reaction;
  os << r.reactionStr(reaction)<< token::END_STATEMENT <<nl;
 return os;
}
}  // namespace mousse
