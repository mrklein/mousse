#ifndef MESH_TOOLS_TRI_SURFACE_BOOLEAN_OPS_SURFACE_INTERSECTION_SURFACE_INTERSECTION_HPP_
#define MESH_TOOLS_TRI_SURFACE_BOOLEAN_OPS_SURFACE_INTERSECTION_SURFACE_INTERSECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceIntersection
// Description
//   Basic surface-surface intersection description. Constructed from two
//   surfaces it creates a description of the intersection.
//   The intersection information consists of the intersection line(s)
//   with new points, new edges between points (note that these edges and
//   points are on both surfaces) and various addressing from original
//   surface faces/edges to intersection and vice versa.
//   Gets either precalculated intersection information or calculates it
//   itself.
//   Algorithm works by intersecting all edges of one surface with the other
//   surface and storing a reference from both faces (one on surface1, one on
//   surface 2) to the vertex. If the reference re-occurs we have the second
//   hit of both faces and an edge is created between the retrieved vertex and
//   the new one.
//   Note: when doing intersecting itself uses intersection::planarTol() as a
//   fraction of
//   current edge length to determine if intersection is a point-touching one
//   instead of an edge-piercing action.

#include "dynamic_list.hpp"
#include "point.hpp"
#include "edge.hpp"
#include "label_pair_lookup.hpp"
#include "type_info.hpp"
#include "edge_list.hpp"
#include "point_index_hit.hpp"


namespace mousse {

// Forward declaration of classes
class triSurfaceSearch;
class triSurface;
class edgeIntersections;


class surfaceIntersection
{
  // Private data

    //- Newly introduced points.
    pointField cutPoints_;

    //- Newly introduced edges (are on both surfaces). Reference into
    //  cutPoints.
    edgeList cutEdges_;

    //- From face on surf1 and face on surf2 to intersection point
    // (label in cutPoints)
    labelPairLookup facePairToVertex_;

    //- From face on surf1 and face on surf2 to intersection edge
    //  (label in cutEdges)
    labelPairLookup facePairToEdge_;

    //- Edges on surf1 that are cut. From edge on surf1 to label in cutPoint
    //  If multiple cuts:sorted from edge.start to edge.end
    labelListList surf1EdgeCuts_;

    //- Edges on surf2 that are cut. From edge on surf2 to label in cutPoint
    //  If multiple cuts:sorted from edge.start to edge.end
    labelListList surf2EdgeCuts_;

  // Private Member Functions

    //- Write point in obj format.
    static void writeOBJ(const point& pt, Ostream& os);

    //- Write points and edges in obj format
    static void writeOBJ
    (
      const List<point>&,
      const List<edge>&,
      Ostream&
    );

    //- Transfer contents of List<DynamicList<..> > to List<List<..>>
    template<class T>
    static void transfer(List<DynamicList<T> >&,  List<List<T> >&);

    //- Get minimum length of all edges connected to point
    static scalar minEdgeLen(const triSurface& surf, const label pointI);

    //- Get edge label of edge between face vertices fp and fp+1
    static label getEdge
    (
      const triSurface& surf,
      const label faceI,
      const label fp
    );

    //- Remove duplicates from ordered dynamic list. Returns map from old
    //  to new (-1 if element removed)
    static void removeDuplicates(const labelList& map, labelList& labels);

    //- Apply map to elements of a labelList
    static void inlineRemap(const labelList& map, labelList& elems);

    // Remove all duplicate and degenerate elements. Return unique elements
    // and map from old to new.
    static edgeList filterEdges(const edgeList&, labelList& map);

    //- Remove all duplicate elements.
    static labelList filterLabels(const labelList& elems, labelList& map);

    //- Do some checks if edge and face (resulting from hit)
    //  should not be considered. Returns true if can be discarded.
    static bool excludeEdgeHit
    (
      const triSurface& surf,
      const label edgeI,
      const label faceI,
      const scalar tol
    );

    //- Debugging: Dump intersected edges to stream
    void writeIntersectedEdges
    (
      const triSurface& surf,
      const labelListList& edgeCutVerts,
      Ostream& os
    ) const;

    //- Detect if point close to edge of end. Returns -1: not close.
    //  0:close (within startTol) to start, 1:close (within endTol) to end
    static label classify
    (
      const scalar startTol,
      const scalar endTol,
      const point& p,
      const edge& e,
      const pointField& points
    );

    //- Update reference between faceA and faceB. Updates facePairToVertex_
    //  (first occurrence of face pair) and facePairToEdge_ (second occ.)
    void storeIntersection
    (
      const bool isFirstSurf,
      const labelList& facesA,
      const label faceB,
      DynamicList<edge>&,
      DynamicList<point>&
    );

    //- Investigate pHit to whether is case of point hits point,
    //  point hits edge, point hits face or edge hits face.
    void classifyHit
    (
      const triSurface& surf1,
      const scalarField& surf1PointTol,
      const triSurface& surf2,
      const bool isFirstSurf,
      const label edgeI,
      const scalar tolDim,
      const pointIndexHit& pHit,
      DynamicList<edge>& allCutEdges,
      DynamicList<point>& allCutPoints,
      List<DynamicList<label> >& surfEdgeCuts
    );

    //- Cut edges of surf1 with surface 2.
    void doCutEdges
    (
      const triSurface& surf1,
      const triSurfaceSearch& querySurf2,
      const bool isFirstSurf,
      const bool isSelfIntersection,
      DynamicList<edge>& allCutEdges,
      DynamicList<point>& allCutPoints,
      List<DynamicList<label> >& surfEdgeCuts
    );

public:

  CLASS_NAME("surfaceIntersection");

  // Constructors

    //- Construct null
    surfaceIntersection();

    //- Construct from precalculated intersection information.
    //  Advantage: intersection information is guaranteed to have no
    //  degenerate cuts.
    surfaceIntersection
    (
      const triSurface& surf1,
      const edgeIntersections& intersections1,
      const triSurface& surf2,
      const edgeIntersections& intersections2
    );

    //- Construct from two surfaces. Does all its own cutting.
    //  Has problems with degenerate cuts
    surfaceIntersection
    (
      const triSurfaceSearch& querySurf1,
      const triSurfaceSearch& querySurf2
    );

    //- Special: intersect surface with itself. Used to check for
    //  self-intersection.
    surfaceIntersection(const triSurfaceSearch& querySurf1);

  // Member Functions
    const pointField& cutPoints() const;
    const edgeList& cutEdges() const;
    const labelPairLookup& facePairToVertex() const;
    const labelPairLookup& facePairToEdge() const;
    //- Access either surf1EdgeCuts (isFirstSurface = true) or
    //  surf2EdgeCuts
    const labelListList& edgeCuts(const bool) const;
    const labelListList& surf1EdgeCuts() const;
    const labelListList& surf2EdgeCuts() const;
};
}  // namespace mousse


#include "surface_intersection.ipp"

#endif
