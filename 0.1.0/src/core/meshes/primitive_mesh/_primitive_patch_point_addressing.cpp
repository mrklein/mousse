// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_primitive_patch.hpp"
#include "sl_list.hpp"
#include "list_ops.hpp"

// Private Member Functions 
template
<
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
void
mousse::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcPointEdges() const
{
  if (debug)
  {
    Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
      << "calcPointEdges() : calculating pointEdges"
      << endl;
  }
  if (pointEdgesPtr_)
  {
    // it is considered an error to attempt to recalculate
    // if already allocated
    FATAL_ERROR_IN
    (
      "PrimitivePatch<Face, FaceList, PointField, PointType>::"
      "calcPointEdges()"
    )
    << "pointEdges already calculated"
    << abort(FatalError);
  }
  pointEdgesPtr_ = new labelListList{meshPoints().size()};
  labelListList& pe = *pointEdgesPtr_;
  invertManyToMany(pe.size(), edges(), pe);
  if (debug)
  {
    Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
      << "calcPointEdges() finished calculating pointEdges"
      << endl;
  }
}


template
<
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
void
mousse::PrimitivePatch<Face, FaceList, PointField, PointType>::
calcPointFaces() const
{
  if (debug)
  {
    Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
      << "calcPointFaces() : calculating pointFaces"
      << endl;
  }
  if (pointFacesPtr_)
  {
    // it is considered an error to attempt to recalculate
    // if already allocated
    FATAL_ERROR_IN
    (
      "PrimitivePatch<Face, FaceList, PointField, PointType>::"
      "calcPointFaces()"
    )
    << "pointFaces already calculated"
    << abort(FatalError);
  }
  const List<Face>& f = localFaces();
  // set up storage for pointFaces
  List<SLList<label>> pointFcs{meshPoints().size()};
  FOR_ALL(f, faceI)
  {
    const Face& curPoints = f[faceI];
    FOR_ALL(curPoints, pointI)
    {
      pointFcs[curPoints[pointI]].append(faceI);
    }
  }
  // sort out the list
  pointFacesPtr_ = new labelListList{pointFcs.size()};
  labelListList& pf = *pointFacesPtr_;
  FOR_ALL(pointFcs, pointI)
  {
    pf[pointI].setSize(pointFcs[pointI].size());
    label i = 0;
    FOR_ALL_ITER(SLList<label>, pointFcs[pointI], curFacesIter)
    {
      pf[pointI][i++] = curFacesIter();
    }
  }
  if (debug)
  {
    Info<< "PrimitivePatch<Face, FaceList, PointField, PointType>::"
      << "calcPointFaces() finished calculating pointFaces"
      << endl;
  }
}
