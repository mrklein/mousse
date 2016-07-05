// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_pair_key.hpp"
// Constructors 
mousse::phasePairKey::hash::hash()
{}
mousse::phasePairKey::phasePairKey()
{}
mousse::phasePairKey::phasePairKey
(
  const word& name1,
  const word& name2,
  const bool ordered
)
:
  Pair<word>{name1, name2},
  ordered_{ordered}
{}
// Destructor 
mousse::phasePairKey::~phasePairKey()
{}
// Member Operators 
mousse::label mousse::phasePairKey::hash::operator()
(
  const phasePairKey& key
) const
{
  if (key.ordered_)
  {
    return word::hash()(key.first(), word::hash()(key.second()));
  }
  else
  {
    return word::hash()(key.first()) + word::hash()(key.second());
  }
}
// Friend Operators 
bool mousse::operator==
(
  const phasePairKey& a,
  const phasePairKey& b
)
{
  const label c = Pair<word>::compare(a,b);
  return
    (a.ordered_ == b.ordered_)
    && ((a.ordered_ && (c == 1)) || (!a.ordered_ && (c != 0)));
}
bool mousse::operator!=
(
  const phasePairKey& a,
  const phasePairKey& b
)
{
  return !(a == b);
}
// Istream Operator 
mousse::Istream& mousse::operator>>(Istream& is, phasePairKey& key)
{
  const FixedList<word, 3> temp{is};
  key.first() = temp[0];
  if (temp[1] == "and")
  {
    key.ordered_ = false;
  }
  else if(temp[1] == "in")
  {
    key.ordered_ = true;
  }
  else
  {
    FATAL_ERROR_IN
    (
      "friend Istream& operator>>"
      "("
        "Istream& is, "
        "phasePairKey& key"
      ")"
    )
    << "Phase pair type is not recognised. "
    << temp
    << "Use (phaseDispersed in phaseContinuous) for an ordered"
    << "pair, or (phase1 and pase2) for an unordered pair."
    << exit(FatalError);
  }
  key.second() = temp[2];
  return is;
}
// Ostream Operator 
mousse::Ostream& mousse::operator<<(Ostream& os, const phasePairKey& key)
{
  os << token::BEGIN_LIST
    << key.first()
    << token::SPACE
    << (key.ordered_ ? "in" : "and")
    << token::SPACE
    << key.second()
    << token::END_LIST;
  return os;
}
