// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_tools.hpp"

template
<
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
mousse::labelListList
mousse::PatchTools::sortedPointEdges
(
  const PrimitivePatch<Face, FaceList, PointField, PointType>& p
)
{
  // Now order the edges of each point according to whether they share a
  // face
  const labelListList& pointEdges = p.pointEdges();
  const edgeList& edges = p.edges();
  const labelListList& edgeFaces = p.edgeFaces();
  const labelListList& faceEdges = p.faceEdges();
  // create the lists for the various results. (resized on completion)
  labelListList sortedPointEdges{pointEdges};
  DynamicList<label> newEdgeList;
  FOR_ALL(pointEdges, pointI)
  {
    const labelList& pEdges = pointEdges[pointI];
    label nPointEdges = pEdges.size();
    label edgeI = pEdges[0];
    label prevFaceI = edgeFaces[edgeI][0];
    newEdgeList.clear();
    newEdgeList.setCapacity(nPointEdges);
    label nVisitedEdges = 0;
    do
    {
      newEdgeList.append(edgeI);
      // Cross edge to next face
      const labelList& eFaces = edgeFaces[edgeI];
      if (eFaces.size() != 2)
      {
        break;
      }
      label faceI = eFaces[0];
      if (faceI == prevFaceI)
      {
        faceI = eFaces[1];
      }
      // Cross face to next edge
      const labelList& fEdges = faceEdges[faceI];
      FOR_ALL(fEdges, feI)
      {
        const label nextEdgeI = fEdges[feI];
        const edge& nextEdge = edges[nextEdgeI];
        if (nextEdgeI != edgeI
            && (nextEdge.start() == pointI || nextEdge.end() == pointI))
        {
          edgeI = nextEdgeI;
          break;
        }
      }
      prevFaceI = faceI;
      nVisitedEdges++;
      if (nVisitedEdges > nPointEdges)
      {
        WARNING_IN("mousse::PatchTools::sortedPointEdges()")
          << "Unable to order pointEdges as the face connections "
          << "are not circular" << nl
          << "    Original pointEdges = " << pEdges << nl
          << "    New pointEdges = " << newEdgeList
          << endl;
        newEdgeList = pEdges;
        break;
      }
    } while (edgeI != pEdges[0]);
    if (newEdgeList.size() == nPointEdges)
    {
      FOR_ALL(pEdges, eI)
      {
        if (findIndex(newEdgeList, pEdges[eI]) == -1)
        {
          WARNING_IN("mousse::PatchTools::sortedPointEdges()")
            << "Cannot find all original edges in the new list"
            << nl
            << "    Original pointEdges = " << pEdges << nl
            << "    New pointEdges = " << newEdgeList
            << endl;
          newEdgeList = pEdges;
          break;
        }
      }
      sortedPointEdges[pointI] = newEdgeList;
    }
  }
  return sortedPointEdges;
}
