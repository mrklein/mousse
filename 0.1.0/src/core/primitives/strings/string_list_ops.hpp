#ifndef CORE_PRIMITIVES_STRINGS_STRING_LIST_OPS_HPP_
#define CORE_PRIMITIVES_STRINGS_STRING_LIST_OPS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   string_list_ops_templates.cpp
#include "reg_exp.hpp"
#include "label_list.hpp"
#include "string_list.hpp"
#include "word_re_list.hpp"
#include "word_re_list_matcher.hpp"
namespace mousse
{
  // single-string matches:
  //- Return true if string matches one of the regular expressions
  inline bool findStrings
  (
    const wordReListMatcher& matcher,
    const std::string& str
  )
  {
    return matcher.match(str);
  }
  // multi-string matches:
  //- Return list indices for matching strings
  template<class Matcher, class StringType>
  labelList findMatchingStrings
  (
    const Matcher&,
    const UList<StringType>&,
    const bool invert=false
  );
  //- Return list indices for strings matching the regular expression
  //  Template partial specialization of findMatchingStrings
  template<class StringType>
  labelList findStrings
  (
    const regExp& re,
    const UList<StringType>& lst,
    const bool invert=false
  )
  {
    return findMatchingStrings(re, lst, invert);
  }
  //- Return list indices for strings matching the regular expression
  //  Template partial specialization of findMatchingStrings
  template<class StringType>
  labelList findStrings
  (
    const char* rePattern,
    const UList<StringType>& lst,
    const bool invert=false
  )
  {
    const regExp re(rePattern);
    return findStrings(re, lst, invert);
  }
  //- Return list indices for strings matching the regular expression
  //  Template partial specialization of findMatchingStrings
  template<class StringType>
  labelList findStrings
  (
    const std::string& rePattern,
    const UList<StringType>& lst,
    const bool invert=false
  )
  {
    const regExp re(rePattern);
    return findMatchingStrings(re, lst, invert);
  }
  //- Return list indices for strings matching the regular expression
  //  Template partial specialization of findMatchingStrings
  template<class StringType>
  labelList findStrings
  (
    const wordRe& wre,
    const UList<StringType>& lst,
    const bool invert=false
  )
  {
    return findMatchingStrings(wre, lst, invert);
  }
  //- Return list indices for strings matching one of the regular expression
  //  Template partial specialization of findMatchingStrings
  template<class StringType>
  labelList findStrings
  (
    const wordReListMatcher& matcher,
    const UList<StringType>& lst,
    const bool invert=false
  )
  {
    return findMatchingStrings(matcher, lst, invert);
  }
  // subsetting multi-string matches (similar to ListOp):
  //- Extract elements of StringList when regular expression matches
  //  optionally invert the match
  //  eg, to extract all selected elements:
  //    subsetMatchingStrings<regExp, stringList>(myRegExp, lst);
  template<class Matcher, class StringListType>
  StringListType subsetMatchingStrings
  (
    const Matcher&,
    const StringListType&,
    const bool invert=false
  );
  //- Extract elements of StringList when regular expression matches
  //  Template partial specialization of subsetMatchingStrings
  template<class StringListType>
  StringListType subsetStrings
  (
    const regExp& re,
    const StringListType& lst,
    const bool invert=false
  )
  {
    return subsetMatchingStrings(re, lst, invert);
  }
  //- Extract elements of StringList when regular expression matches
  //  Template partial specialization of subsetMatchingStrings
  template<class StringListType>
  StringListType subsetStrings
  (
    const char* rePattern,
    const StringListType& lst,
    const bool invert=false
  )
  {
    const regExp re(rePattern);
    return subsetMatchingStrings(re, lst, invert);
  }
  //- Extract elements of StringList when regular expression matches
  //  Template partial specialization of subsetMatchingStrings
  template<class StringListType>
  StringListType subsetStrings
  (
    const std::string& rePattern,
    const StringListType& lst,
    const bool invert=false
  )
  {
    const regExp re(rePattern);
    return subsetMatchingStrings(re, lst, invert);
  }
  //- Extract elements of StringList when regular expression matches
  //  Template partial specialization of subsetMatchingStrings
  template<class StringListType>
  StringListType subsetStrings
  (
    const wordRe& wre,
    const StringListType& lst,
    const bool invert=false
  )
  {
    return subsetMatchingStrings(wre, lst, invert);
  }
  //- Extract elements of StringList when regular expression matches
  //  Template partial specialization of subsetMatchingStrings
  template<class StringListType>
  StringListType subsetStrings
  (
    const wordReListMatcher& matcher,
    const StringListType& lst,
    const bool invert=false
  )
  {
    return subsetMatchingStrings(matcher, lst, invert);
  }
  //- Inplace extract elements of StringList when regular expression matches
  //  optionally invert the match
  //  eg, to extract all selected elements:
  //    inplaceSubsetMatchingStrings<regExp, stringList>(myRegExp, lst);
  template<class Matcher, class StringListType>
  void inplaceSubsetMatchingStrings
  (
    const Matcher&,
    StringListType&,
    const bool invert=false
  );
  //- Inplace extract elements of StringList when regular expression matches
  //  Template partial specialization of inplaceSubsetMatchingStrings
  template<class StringListType>
  void inplaceSubsetStrings
  (
    const regExp& re,
    StringListType& lst,
    const bool invert=false
  )
  {
    inplaceSubsetMatchingStrings(re, lst, invert);
  }
  //- Inplace extract elements of StringList when regular expression matches
  //  Template partial specialization of inplaceSubsetMatchingStrings
  template<class StringListType>
  void inplaceSubsetStrings
  (
    const char* rePattern,
    StringListType& lst,
    const bool invert=false
  )
  {
    const regExp re(rePattern);
    inplaceSubsetMatchingStrings(re, lst, invert);
  }
  //- Inplace extract elements of StringList when regular expression matches
  //  Template partial specialization of inplaceSubsetMatchingStrings
  template<class StringListType>
  void inplaceSubsetStrings
  (
    const std::string& rePattern,
    StringListType& lst,
    const bool invert=false
  )
  {
    const regExp re(rePattern);
    inplaceSubsetMatchingStrings(re, lst, invert);
  }
  //- Inplace extract elements of StringList when regular expression matches
  //  Template partial specialization of inplaceSubsetMatchingStrings
  template<class StringListType>
  void inplaceSubsetStrings
  (
    const wordRe& wre,
    StringListType& lst,
    const bool invert=false
  )
  {
    inplaceSubsetMatchingStrings(wre, lst, invert);
  }
  //- Inplace extract elements of StringList when regular expression matches
  //  Template partial specialization of inplaceSubsetMatchingStrings
  template<class StringListType>
  void inplaceSubsetStrings
  (
    const wordReListMatcher& matcher,
    StringListType& lst,
    const bool invert=false
  )
  {
    inplaceSubsetMatchingStrings(matcher, lst, invert);
  }
}
#ifdef NoRepository
#   include "string_list_ops_templates.cpp"
#endif
#endif
