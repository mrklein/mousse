// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Matcher, class StringType>
mousse::labelList mousse::findMatchingStrings
(
  const Matcher& matcher,
  const UList<StringType>& lst,
  const bool invert
)
{
  labelList indices{lst.size()};
  label nElem = 0;
  FOR_ALL(lst, elemI)
  {
    if (matcher.match(lst[elemI]) ? !invert : invert)
    {
      indices[nElem++] = elemI;
    }
  }
  indices.setSize(nElem);
  return indices;
}


template<class Matcher, class StringListType>
StringListType mousse::subsetMatchingStrings
(
  const Matcher& matcher,
  const StringListType& lst,
  const bool invert
)
{
  // Create copy
  StringListType newLst{lst.size()};
  // ensure consistent addressable size (eg, DynamicList)
  newLst.setSize(lst.size());
  label nElem = 0;
  FOR_ALL(lst, elemI)
  {
    if (matcher.match(lst[elemI]) ? !invert : invert)
    {
      newLst[nElem++] = lst[elemI];
    }
  }
  newLst.setSize(nElem);
  return newLst;
}


template<class Matcher, class StringListType>
void mousse::inplaceSubsetMatchingStrings
(
  const Matcher& matcher,
  StringListType& lst,
  const bool invert
)
{
  label nElem = 0;
  FOR_ALL(lst, elemI)
  {
    if (matcher.match(lst[elemI]) ? !invert : invert)
    {
      lst[nElem++] = lst[elemI];
    }
  }
  lst.setSize(nElem);
}
