// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_tools.hpp"
#include "sortable_list.hpp"
#include "transform.hpp"


template
<
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
mousse::labelListList
mousse::PatchTools::sortedEdgeFaces
(
  const PrimitivePatch<Face, FaceList, PointField, PointType>& p
)
{
  const edgeList& edges = p.edges();
  const labelListList& edgeFaces = p.edgeFaces();
  const List<Face>& localFaces = p.localFaces();
  const Field<PointType>& localPoints = p.localPoints();
  // create the lists for the various results. (resized on completion)
  labelListList sortedEdgeFaces{edgeFaces.size()};
  FOR_ALL(edgeFaces, edgeI) {
    const labelList& faceNbs = edgeFaces[edgeI];
    if (faceNbs.size() > 2) {
      // Get point on edge and normalized direction of edge (= e2 base
      // of our coordinate system)
      const edge& e = edges[edgeI];
      const point& edgePt = localPoints[e.start()];
      vector e2 = e.vec(localPoints);
      e2 /= mag(e2) + VSMALL;
      // Get the vertex on 0th face that forms a vector with the first
      // edge point that has the largest angle with the edge
      const Face& f0 = localFaces[faceNbs[0]];
      scalar maxAngle = GREAT;
      vector maxAngleEdgeDir{vector::max};
      FOR_ALL(f0, fpI) {
        if (f0[fpI] != e.start()) {
          vector faceEdgeDir = localPoints[f0[fpI]] - edgePt;
          faceEdgeDir /= mag(faceEdgeDir) + VSMALL;
          const scalar angle = e2 & faceEdgeDir;
          if (mag(angle) < maxAngle) {
            maxAngle = angle;
            maxAngleEdgeDir = faceEdgeDir;
          }
        }
      }
      // Get vector normal both to e2 and to edge from opposite vertex
      // to edge (will be x-axis of our coordinate system)
      vector e0 = e2 ^ maxAngleEdgeDir;
      e0 /= mag(e0) + VSMALL;
      // Get y-axis of coordinate system
      vector e1 = e2 ^ e0;
      SortableList<scalar> faceAngles{faceNbs.size()};
      // e0 is reference so angle is 0
      faceAngles[0] = 0;
      for (label nbI = 1; nbI < faceNbs.size(); nbI++) {
        // Get the vertex on face that forms a vector with the first
        // edge point that has the largest angle with the edge
        const Face& f = localFaces[faceNbs[nbI]];
        maxAngle = GREAT;
        maxAngleEdgeDir = vector::max;
        FOR_ALL(f, fpI) {
          if (f[fpI] != e.start()) {
            vector faceEdgeDir = localPoints[f[fpI]] - edgePt;
            faceEdgeDir /= mag(faceEdgeDir) + VSMALL;
            const scalar angle = e2 & faceEdgeDir;
            if (mag(angle) < maxAngle) {
              maxAngle = angle;
              maxAngleEdgeDir = faceEdgeDir;
            }
          }
        }
        vector vec = e2 ^ maxAngleEdgeDir;
        vec /= mag(vec) + VSMALL;
        faceAngles[nbI] = pseudoAngle
        (
          e0,
          e1,
          vec
        );
      }
      faceAngles.sort();
      sortedEdgeFaces[edgeI] = UIndirectList<label>
      (
        faceNbs,
        faceAngles.indices()
      );
    } else {
      // No need to sort. Just copy.
      sortedEdgeFaces[edgeI] = faceNbs;
    }
  }
  return sortedEdgeFaces;
}
