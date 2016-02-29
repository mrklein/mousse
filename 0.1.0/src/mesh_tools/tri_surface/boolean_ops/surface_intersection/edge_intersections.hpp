#ifndef MESH_TOOLS_TRI_SURFACE_BOOLEAN_OPS_SURFACE_INTERSECTION_EDGE_INTERSECTIONS_HPP_
#define MESH_TOOLS_TRI_SURFACE_BOOLEAN_OPS_SURFACE_INTERSECTION_EDGE_INTERSECTIONS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::edgeIntersections
// Description
//   Holder of intersections of edges of a surface with another surface.
//   Optionally shuffles around points on surface to resolve any 'conflicts'
//   (edge hitting triangle edge, edge hitting point etc.).
// SourceFiles
//   edge_intersections.cpp
#include "point_index_hit.hpp"
#include "scalar_field.hpp"
#include "point_field.hpp"
#include "type_info.hpp"
#include "bool_list.hpp"
namespace mousse
{
// Forward declaration of classes
class triSurface;
class triSurfaceSearch;
class Random;
class edge;
class edgeIntersections
:
  public List<List<pointIndexHit> >
{
  // Private data
    //- For every entry in *this gives the edge classification result.
    //  -1 : intersection not close to edge
    //   0 : intersection close to e[0]
    //   1 : intersection close to e[1]
    //   2 : edge aligned with intersection face
    labelListList classification_;
  // Private Member Functions
    //- Check for too small edges
    static void checkEdges(const triSurface& surf);
    //- Intersect selected surface edges (edgeLabels) with surface2.
    //  Updates *this with pointHits and classification_ with status
    //  of hitPoint compared to edge end points.
    void intersectEdges
    (
      const triSurface& surf1,
      const pointField& points1,          // surf1 meshPoints
      const triSurfaceSearch& querySurf2,
      const scalarField& surf1PointTol,   // surf1 tolerance per point
      const labelList& edgeLabels
    );
    //- Perturb endpoints of edge if they are close to the intersection.
    //  Move point (in points1) by factor*surf1PointTol in direction of
    //  edge. Mark pointEdges of moved point in affectedEdges.
    //  Return true if anything changed.
    bool inlinePerturb
    (
      const triSurface& surf1,
      const scalarField& surf1PointTol,
      const label edgeI,
      Random& rndGen,
      pointField& points1,
      boolList& affectedEdges
    ) const;
    //- Perturb single endpoint of edge if edge is algigned with face.
    //  See inlinePerturb. Return true if anything changed.
    bool rotatePerturb
    (
      const triSurface& surf1,
      const scalarField& surf1PointTol,
      const label edgeI,
      Random& rndGen,
      pointField& points1,
      boolList& affectedEdges
    ) const;
    //- Perturb edge by shifting in direction trianglecentre - intersection
    //  when hits close to face. Update points, mark affected edges and
    //  return true if anything changed.
    bool offsetPerturb
    (
      const triSurface& surf1,
      const triSurface& surf2,
      const label edgeI,
      Random& rndGen,
      pointField& points1,
      boolList& affectedEdges
    ) const;
public:
  CLASS_NAME("edgeIntersections");
  // Static data members
    //- Cosine between edge and face normal when considered parallel
    //  (note: should be private and make access- and set- function)
    static scalar alignedCos_;
  // Static Functions
    //- Calculate min edge length for every surface point
    static scalarField minEdgeLength(const triSurface& surf);
  // Constructors
    //- Construct null
    edgeIntersections();
    //- Construct from surface and tolerance
    edgeIntersections
    (
      const triSurface& surf1,
      const triSurfaceSearch& query2,
      const scalarField& surf1PointTol
    );
    //- Construct from components
    edgeIntersections
    (
      const List<List<pointIndexHit> >&,
      const labelListList&
    );
  // Member Functions
    // Access
      //- For every intersection the classification status.
      const labelListList& classification() const
      {
        return classification_;
      }
    // Edit
      //- Resolve ties. Shuffles points so all edge - face intersections
      //  will be on the face interior.
      //  Points will be the new surface points.
      //  Returns number of iterations needed. (= nIters if still
      //  has degenerate cuts)
      label removeDegenerates
      (
        const label nIters,
        const triSurface& surf1,
        const triSurfaceSearch& query2,
        const scalarField& surf1PointTol,
        pointField& points1
      );
      //- Merge (or override) edge intersection for a subset
      //  (given as edge map and face map - for face indices stored in
      //  pointIndexHit.index())
      void merge
      (
        const edgeIntersections&,
        const labelList& edgeMap,
        const labelList& faceMap,
        const bool merge = true
      );
};
}  // namespace mousse
#endif
