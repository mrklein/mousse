// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_intersection.hpp"


// Private Member Functions 

// Transfer contents of DynamicList to List
template<class T>
void mousse::surfaceIntersection::transfer
(
  List<DynamicList<T> >& srcLst,
  List<List<T> >& dstLst
)
{
  dstLst.setSize(srcLst.size());
  FOR_ALL(srcLst, elemI) {
    dstLst[elemI].transfer(srcLst[elemI]);
  }
}
