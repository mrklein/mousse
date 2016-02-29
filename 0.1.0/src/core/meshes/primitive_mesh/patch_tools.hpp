#ifndef CORE_MESHES_PRIMITIVE_MESH_PATCH_TOOLS_HPP_
#define CORE_MESHES_PRIMITIVE_MESH_PATCH_TOOLS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PatchTools
// Description
//   A collection of tools for searching, sorting PrimitivePatch information.
//   The class could also be extended to include more that just static methods.
// SourceFiles
//   patch_tools.cpp
//   patch_tools_check.cpp
//   patch_tools_edge_owner.cpp
//   patch_tools_gather_and_merge.cpp
//   patch_tools_match.cpp
//   patch_tools_normals.cpp
//   patch_tools_search.cpp
//   patch_tools_sort_edges.cpp
//   patch_tools_sort_points.cpp


#include "primitive_patch.hpp"
#include "hash_set.hpp"

namespace mousse
{
class polyMesh;
class PackedBoolList;
class boundBox;

class PatchTools
{
public:

  //- Check for orientation issues.
  //  Returns true if problems were found.
  //  If a normal flips across an edge, places it in the HashSet
  template
  <
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
  >
  static bool checkOrientation
  (
    const PrimitivePatch<Face, FaceList, PointField, PointType>&,
    const bool report = false,
    labelHashSet* marked = 0
  );

  //- Fill faceZone with currentZone for every face reachable
  //  from faceI without crossing edge marked in borderEdge.
  //  Note: faceZone has to be sized nFaces before calling.
  template
  <
    class BoolListType,
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
  >
  static void markZone
  (
    const PrimitivePatch<Face, FaceList, PointField, PointType>&,
    const BoolListType& borderEdge,
    const label faceI,
    const label currentZone,
    labelList& faceZone
  );

  //- Size and fills faceZone with zone of face.
  //  Zone is area reachable by edge crossing without crossing borderEdge.
  //  Returns number of zones.
  template
  <
    class BoolListType,
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
  >
  static label markZones
  (
    const PrimitivePatch<Face, FaceList, PointField, PointType>&,
    const BoolListType& borderEdge,
    labelList& faceZone
  );

  //- Determine the mapping for a sub-patch.
  //  Only include faces for which bool-list entry is true.
  //  \param[in]  includeFaces faces to include
  //  \param[out] pointMap mapping new to old localPoints
  //  \param[out] faceMap  mapping new to old faces
  template
  <
    class BoolListType,
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
  >
  static void subsetMap
  (
    const PrimitivePatch<Face, FaceList, PointField, PointType>&,
    const BoolListType& includeFaces,
    labelList& pointMap,
    labelList& faceMap
  );

  //-
  template
  <
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
  >
  static void calcBounds
  (
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
    boundBox& bb,
    label& nPoints
  );

  //- Return edge-face addressing sorted by angle around the edge.
  //  Orientation is anticlockwise looking from edge.vec(localPoints())
  template
  <
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
  >
  static labelListList sortedEdgeFaces
  (
    const PrimitivePatch<Face, FaceList, PointField, PointType>&
  );

  //- Return point-edge addressing sorted by order around the point.
  template
  <
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
  >
  static labelListList sortedPointEdges
  (
    const PrimitivePatch<Face, FaceList, PointField, PointType>&
  );

  //- If 2 face neighbours: label of face where ordering of edge
  //  is consistent with righthand walk.
  //  If 1 neighbour: label of only face.
  //  If >2 neighbours: undetermined.
  template
  <
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
  >
  static labelList edgeOwner
  (
    const PrimitivePatch<Face, FaceList, PointField, PointType>&
  );

  //- Find corresponding points on patches sharing the same points
  //  p1PointLabels : points on p1 that were matched
  //  p2PointLabels : corresponding points on p2
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
  static void matchPoints
  (
    const PrimitivePatch<Face1, FaceList1, PointField1, PointType1>& p1,
    const PrimitivePatch<Face2, FaceList2, PointField2, PointType2>& p2,
    labelList& p1PointLabels,
    labelList& p2PointLabels
  );

  //- Find corresponding edges on patches sharing the same points
  //  p1EdgeLabels    : edges on p1 that were matched
  //  p2EdgeLabels    : corresponding edges on p2
  //  sameOrientation : same orientation?
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
  static void matchEdges
  (
    const PrimitivePatch<Face1, FaceList1, PointField1, PointType1>& p1,
    const PrimitivePatch<Face2, FaceList2, PointField2, PointType2>& p2,
    labelList& p1EdgeLabels,
    labelList& p2EdgeLabels,
    PackedBoolList& sameOrientation
  );

  //- Return parallel consistent point normals for patches using mesh points.
  template
  <
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
  >
  static tmp<pointField> pointNormals
  (
    const polyMesh&,
    const PrimitivePatch<Face, FaceList, PointField, PointType>&
  );

  //- Return parallel consistent edge normals for patches using mesh points.
  //  Supply with patch matching info from matchEdges.
  template
  <
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
  >
  static tmp<pointField> edgeNormals
  (
    const polyMesh&,
    const PrimitivePatch<Face, FaceList, PointField, PointType>&,
    const labelList& patchEdges,
    const labelList& coupledEdges
  );

  //- Gather points and faces onto master and merge into single patch.
  //  Note: uses faces/points, not localFaces/localPoints.
  template
  <
    class Face,
    template<class> class FaceList,
    class PointField,
    class PointType
  >
  static void gatherAndMerge
  (
    const scalar mergeDist,
    const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
    Field<PointType>& mergedPoints,
    List<Face>& mergedFaces,
    labelList& pointMergeMap
  );
};
}  // namespace mousse
#ifdef NoRepository
#   include "patch_tools.cpp"
#endif
#endif
