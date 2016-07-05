// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "list_ops.hpp"


// Static Data Members
const mousse::labelList mousse::emptyLabelList = mousse::labelList{0};


// Global Functions 
mousse::labelList mousse::invert
(
  const label len,
  const labelUList& map
)
{
  labelList inverse{len, -1};
  FOR_ALL(map, i) {
    label newPos = map[i];
    if (newPos >= 0) {
      if (inverse[newPos] >= 0) {
        FATAL_ERROR_IN("invert(const label, const labelUList&)")
          << "Map is not one-to-one. At index " << i
          << " element " << newPos << " has already occurred before"
          << nl << "Please use invertOneToMany instead"
          << abort(FatalError);
      }
      inverse[newPos] = i;
    }
  }
  return inverse;
}


mousse::labelListList mousse::invertOneToMany
(
  const label len,
  const labelUList& map
)
{
  labelList nElems{len, 0};
  FOR_ALL(map, i) {
    if (map[i] >= 0) {
      nElems[map[i]]++;
    }
  }
  labelListList inverse(len);
  FOR_ALL(nElems, i) {
    inverse[i].setSize(nElems[i]);
    nElems[i] = 0;
  }
  FOR_ALL(map, i) {
    label newI = map[i];
    if (newI >= 0) {
      inverse[newI][nElems[newI]++] = i;
    }
  }
  return inverse;
}


mousse::labelList mousse::identity(const label len)
{
  labelList map{len};
  FOR_ALL(map, i) {
    map[i] = i;
  }
  return map;
}
