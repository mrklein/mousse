// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wordReListMatcher
// Description
//   A wrapper for matching a List of wordRe.
// Note
//   The constructor should remain non-explicit. This allows automatic
//   conversion from UList\<wordRe\> to wordReListMatcher in search
//   functions.
#ifndef word_re_list_matcher_hpp_
#define word_re_list_matcher_hpp_
#include "word_re_list.hpp"
namespace mousse
{
class wordReListMatcher
{
  // Private data
    //- Reference to underlying list
    const UList<wordRe>& reList_;
public:
  // Constructors
    //- Construct from a List of wordRe
    inline wordReListMatcher(const UList<wordRe>&);
  // Member Functions
    // Access
      inline label size() const;
      inline bool  empty() const;
      //- Return underlying list of wordRe
      inline const UList<wordRe>& operator()() const;
  // Searching
      //- Return true if string matches any of the regular expressions
      //  Smart match as regular expression or as a string.
      //  Optionally specify a literal match only.
      inline bool match(const string&, bool literalMatch=false) const;
};
}  // namespace mousse

// Constructors 
inline mousse::wordReListMatcher::wordReListMatcher
(
  const UList<wordRe>& lst
)
:
  reList_{lst}
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
#endif
