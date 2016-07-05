// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_topo_change.hpp"


// Member Functions 
template<class T>
void mousse::polyTopoChange::reorder
(
  const labelList& oldToNew,
  DynamicList<T>& lst
)
{
  // Create copy
  DynamicList<T> oldLst{lst};
  FOR_ALL(oldToNew, elemI) {
    label newElemI = oldToNew[elemI];
    if (newElemI != -1) {
      lst[newElemI] = oldLst[elemI];
    }
  }
}


template<class T>
void mousse::polyTopoChange::reorder
(
  const labelList& oldToNew,
  List<DynamicList<T>>& lst
)
{
  // Create copy
  List<DynamicList<T>> oldLst{lst};
  FOR_ALL(oldToNew, elemI) {
    label newElemI = oldToNew[elemI];
    if (newElemI != -1) {
      lst[newElemI].transfer(oldLst[elemI]);
    }
  }
}


template<class T>
void mousse::polyTopoChange::renumberKey
(
  const labelList& oldToNew,
  Map<T>& elems
)
{
  Map<T> newElems{elems.size()};
  FOR_ALL_CONST_ITER(typename Map<T>, elems, iter) {
    label newElem = oldToNew[iter.key()];
    if (newElem >= 0) {
      newElems.insert(newElem, iter());
    }
  }
  elems.transfer(newElems);
}

