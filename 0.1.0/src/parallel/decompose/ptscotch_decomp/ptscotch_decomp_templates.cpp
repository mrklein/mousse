// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ptscotch_decomp.hpp"
// Member Functions 
// Insert at front of list
template<class Type>
void mousse::ptscotchDecomp::prepend
(
  const UList<Type>& extraLst,
  List<Type>& lst
)
{
  label nExtra = extraLst.size();
  // Make space for initial elements
  lst.setSize(lst.size() + nExtra);
  for (label i = lst.size()-1; i >= nExtra; i--)
  {
    lst[i] = lst[i-nExtra];
  }
  // Insert at front
  forAll(extraLst, i)
  {
    lst[i] = extraLst[i];
  }
}
// Insert at back of list
template<class Type>
void mousse::ptscotchDecomp::append
(
  const UList<Type>& extraLst,
  List<Type>& lst
)
{
  label sz = lst.size();
  // Make space for initial elements
  lst.setSize(sz + extraLst.size());
  // Insert at back
  forAll(extraLst, i)
  {
    lst[sz++] = extraLst[i];
  }
}
