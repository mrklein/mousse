// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::wordReListMatcher::wordReListMatcher
(
  const UList<wordRe>& lst
)
:
  reList_(lst)
{}
// Member Functions 
inline mousse::label mousse::wordReListMatcher::size() const
{
  return reList_.size();
}
inline bool mousse::wordReListMatcher::empty() const
{
  return reList_.empty();
}
inline const mousse::UList<mousse::wordRe>&
mousse::wordReListMatcher::operator()() const
{
  return reList_;
}
inline bool mousse::wordReListMatcher::match
(
  const string& str,
  bool literalMatch
) const
{
  const label nElem = reList_.size();
  for (label elemI = 0; elemI < nElem; ++elemI)
  {
    if (reList_[elemI].match(str, literalMatch))
    {
      return true;
    }
  }
  return false;
}
