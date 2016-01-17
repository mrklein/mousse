// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_tools.hpp"

template
<
  class Face1,
  template<class> class FaceList1,
  class PointField1,
  class PointType1,
  class Face2,
  template<class> class FaceList2,
  class PointField2,
  class PointType2
>
void mousse::PatchTools::matchPoints
(
  const PrimitivePatch<Face1, FaceList1, PointField1, PointType1>& p1,
  const PrimitivePatch<Face2, FaceList2, PointField2, PointType2>& p2,
  labelList& p1PointLabels,
  labelList& p2PointLabels
)
{
  p1PointLabels.setSize(p1.nPoints());
  p2PointLabels.setSize(p1.nPoints());
  label nMatches = 0;
  FOR_ALL(p1.meshPoints(), pointI)
  {
    label meshPointI = p1.meshPoints()[pointI];
    Map<label>::const_iterator iter = p2.meshPointMap().find
    (
      meshPointI
    );
    if (iter != p2.meshPointMap().end())
    {
      p1PointLabels[nMatches] = pointI;
      p2PointLabels[nMatches] = iter();
      nMatches++;
    }
  }
  p1PointLabels.setSize(nMatches);
  p2PointLabels.setSize(nMatches);
}


template
<
  class Face1,
  template<class> class FaceList1,
  class PointField1,
  class PointType1,
  class Face2,
  template<class> class FaceList2,
  class PointField2,
  class PointType2
>
void mousse::PatchTools::matchEdges
(
  const PrimitivePatch<Face1, FaceList1, PointField1, PointType1>& p1,
  const PrimitivePatch<Face2, FaceList2, PointField2, PointType2>& p2,
  labelList& p1EdgeLabels,
  labelList& p2EdgeLabels,
  PackedBoolList& sameOrientation
)
{
  p1EdgeLabels.setSize(p1.nEdges());
  p2EdgeLabels.setSize(p1.nEdges());
  sameOrientation.setSize(p1.nEdges());
  sameOrientation = 0;
  label nMatches = 0;
  EdgeMap<label> edgeToIndex{2*p1.nEdges()};
  FOR_ALL(p1.edges(), edgeI)
  {
    const edge& e = p1.edges()[edgeI];
    const edge meshE
    {
      p1.meshPoints()[e[0]],
      p1.meshPoints()[e[1]]
    };
    edgeToIndex.insert(meshE, edgeI);
  }
  FOR_ALL(p2.edges(), edgeI)
  {
    const edge& e = p2.edges()[edgeI];
    const edge meshE(p2.meshPoints()[e[0]], p2.meshPoints()[e[1]]);
    EdgeMap<label>::const_iterator iter = edgeToIndex.find(meshE);
    if (iter != edgeToIndex.end())
    {
      p1EdgeLabels[nMatches] = iter();
      p2EdgeLabels[nMatches] = edgeI;
      sameOrientation[nMatches] = (meshE[0] == iter.key()[0]);
      nMatches++;
    }
  }
  p1EdgeLabels.setSize(nMatches);
  p2EdgeLabels.setSize(nMatches);
  sameOrientation.setSize(nMatches);
}
