#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_FOAMY_QUAD_MESH_CV_2D_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_FOAMY_QUAD_MESH_CV_2D_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CV2D
// Description
//   Conformal-Voronoi 2D automatic mesher with grid or read initial points
//   and point position relaxation with optional "squarification".
//   There are a substantial number of options to this mesher read from
//   CV2DMesherDict file e.g.:
//   // Min cell size used in tolerances when inserting points for
//   // boundary conforming.
//   // Also used to as the grid spacing usind in insertGrid.
//   minCellSize  0.05;
//   // Feature angle used to inser feature points
//   // 0 = all features, 180 = no features
//   featureAngle 45;
//   // Maximum quadrant angle allowed at a concave corner before
//   // additional "mitering" lines are added
//   maxQuadAngle 110;
//   // Should the mesh be square-dominated or of unbiased hexagons
//   squares      yes;
//   // Near-wall region where cells are aligned with the wall specified as a
//   // number of cell layers
//   nearWallAlignedDist 3;
//   // Chose if the cell orientation should relax during the iterations
//   //  or remain fixed to the x-y directions
//   relaxOrientation    no;
//   // Insert near-boundary point mirror or point-pairs
//   insertSurfaceNearestPointPairs yes;
//   // Mirror near-boundary points rather than insert point-pairs
//   mirrorPoints   no;
//   // Insert point-pairs vor dual-cell vertices very near the surface
//   insertSurfaceNearPointPairs yes;
//   // Choose if to randomise the initial grid created by insertGrid.
//   randomiseInitialGrid yes;
//   // Perturbation fraction, 1 = cell-size.
//   randomPurturbation   0.1;
//   // Number of relaxation iterations.
//   nIterations      5;
//   // Relaxation factor at the start of the iteration sequence.
//   // 0.5 is a sensible maximum and < 0.2 converges better.
//   relaxationFactorStart 0.8;
//   // Relaxation factor at the end of the iteration sequence.
//   // Should be <= relaxationFactorStart
//   relaxationFactorEnd   0;
//   writeInitialTriangulation no;
//   writeFeatureTriangulation no;
//   writeNearestTriangulation no;
//   writeInsertedPointPairs   no;
//   writeFinalTriangulation   yes;
//   // Maximum number of iterations used in boundaryConform.
//   maxBoundaryConformingIter 5;
//   minEdgeLenCoeff           0.5;
//   maxNotchLenCoeff          0.3;
//   minNearPointDistCoeff     0.25;
//   ppDistCoeff               0.05;

#define CGAL_INEXACT
#define CGAL_HIERARCHY

// Include uint.H before CGAL headers to define __STDC_LIMIT_MACROS
#include "uint.hpp"
#include "cgal_triangulation_2d_defs.hpp"
#include "time.hpp"
#include "point_2d_field_fwd.hpp"
#include "dictionary.hpp"
#include "switch.hpp"
#include "packed_bool_list.hpp"
#include "edge_map.hpp"
#include "cv_2d_controls.hpp"
#include "tolerances.hpp"
#include "mesh_tools.hpp"
#include "tri_surface.hpp"
#include "searchable_surfaces.hpp"
#include "conformation_surfaces.hpp"
#include "relaxation_model.hpp"
#include "cell_size_and_alignment_controls.hpp"


namespace mousse {

class CV2D
:
  public Delaunay
{
private:
  // Private data
    //- The time registry of the application
    const Time& runTime_;
    mutable Random rndGen_;
    //- The surface to mesh
    //const querySurface& qSurf_;
    //- All geometry of the meshing process, including surfaces to be
    //  conformed to and those to be used for refinement
    searchableSurfaces allGeometry_;
    conformationSurfaces qSurf_;
    //- Meshing controls
    cv2DControls controls_;
    //- The cell size control object
    cellSizeAndAlignmentControls cellSizeControl_;
    //- Relaxation coefficient model.  Runtime selectable.
    autoPtr<relaxationModel> relaxationModel_;
    //- z-level
    scalar z_;
    //- Keep track of the start of the internal points
    label startOfInternalPoints_;
    //- Keep track of the start of the surface point-pairs
    label startOfSurfacePointPairs_;
    //- Keep track of the boundary conform point-pairs
    //  stored after the insertion of the surface point-pairs in case
    //  the boundary conform function is called more than once without
    //  removing and insertin the surface point-pairs
    label startOfBoundaryConformPointPairs_;
    //- Store the feature points
    std::list<Vb> featurePoints_;
    //- Temporary storage for a dual-cell
    static const label maxNvert = 20;
    mutable point2D vertices[maxNvert+1];
    mutable vector2D edges[maxNvert+1];
  // Private Member Functions
    //- Insert point and return it's index
    inline label insertPoint
    (
      const point2D& pt,
      const label type
    );
    //- Insert point and return it's index
    inline label insertPoint
    (
      const point2D& pt,
      const label index,
      const label type
    );
    inline label insertPoint
    (
      const Point& p,
      const label index,
      const label type
    );
    inline bool insertMirrorPoint
    (
      const point2D& nearSurfPt,
      const point2D& surfPt
    );
    //- Insert a point-pair at a distance ppDist either side of
    //  surface point point surfPt in the direction n
    inline void insertPointPair
    (
      const scalar mirrorDist,
      const point2D& surfPt,
      const vector2D& n
    );
    //- Create the initial mesh from the bounding-box
    void insertBoundingBox();
    //- Check if a point is within a line.
    bool on2DLine(const point2D& p, const linePointRef& line);
    //- Insert point groups at the feature points.
    void insertFeaturePoints();
    //- Re-insert point groups at the feature points.
    void reinsertFeaturePoints();
    //- Insert point-pairs at the given set of points using the surface
    //  normals corresponding to the given set of surface triangles
    //  and write the inserted point locations to the given file.
    void insertPointPairs
    (
      const DynamicList<point2D>& nearSurfacePoints,
      const DynamicList<point2D>& surfacePoints,
      const DynamicList<label>& surfaceTris,
      const DynamicList<label>& surfaceHits,
      const fileName fName
    );
    //- Check to see if dual cell specified by given vertex iterator
    //  intersects the boundary and hence reqires a point-pair.
    bool dualCellSurfaceIntersection
    (
      const Triangulation::Finite_vertices_iterator& vit
    ) const;
    //- Insert point-pairs at the nearest points on the surface to the
    //  control vertex of dual-cells which intersect the boundary in order
    //  to provide a boundary-layer mesh.
    //  NB: This is not guaranteed to close the boundary
    void insertSurfaceNearestPointPairs();
    //- Insert point-pairs at small dual-cell edges on the surface in order
    //  to improve the boundary-layer mesh generated by
    //  insertSurfaceNearestPointPairs.
    void insertSurfaceNearPointPairs();
    //- Insert point-pair and correcting the Finite_vertices_iterator
    //  to account for the additional vertices
    void insertPointPair
    (
      Triangulation::Finite_vertices_iterator& vit,
      const point2D& p,
      const label trii,
      const label hitSurface
    );
    //- Insert point-pair at the best intersection point between the lines
    //  from the dual-cell real centroid and it's vertices and the surface.
    bool insertPointPairAtIntersection
    (
      Triangulation::Finite_vertices_iterator& vit,
      const point2D& defVert,
      const point2D vertices[],
      const scalar maxProtSize
    );
    //- Insert point-pairs corresponding to dual-cells which intersect
    //  the boundary surface
    label insertBoundaryConformPointPairs(const fileName& fName);
    void markNearBoundaryPoints();
    //- Restore the Delaunay contraint
    void fast_restore_Delaunay(Vertex_handle vh);
    // Flip operations used by fast_restore_Delaunay
    void external_flip(Face_handle& f, int i);
    bool internal_flip(Face_handle& f, int i);
    //- Write all the faces and all the triangles at a particular stage.
    void write(const word& stage) const;
public:
  //- Runtime type information
  CLASS_NAME("CV2D");
  // Constructors
    //- Construct for given surface
    CV2D(const Time& runTime, const dictionary& controlDict);
    //- Disallow default bitwise copy construct
    CV2D(const CV2D&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const CV2D&) = delete;
  //- Destructor
  ~CV2D();
  // Member Functions
    // Access
      inline const cv2DControls& meshControls() const;
    // Conversion functions between point2D, point and Point
      inline const point2D& toPoint2D(const point&) const;
      inline const point2DField toPoint2D(const pointField&) const;
      inline point toPoint3D(const point2D&) const;
      #ifdef CGAL_INEXACT
        typedef const point2D& point2DFromPoint;
        typedef const Point& PointFromPoint2D;
      #else
        typedef point2D point2DFromPoint;
        typedef Point PointFromPoint2D;
      #endif
      inline point2DFromPoint toPoint2D(const Point&) const;
      inline PointFromPoint2D toPoint(const point2D&) const;
      inline point toPoint3D(const Point&) const;
    // Point insertion
      //- Create the initial mesh from the given internal points.
      //  Points must be inside the boundary by at least nearness
      //  otherwise they are ignored.
      void insertPoints
      (
        const point2DField& points,
        const scalar nearness
      );
      //- Create the initial mesh from the internal points in the given
      //  file.  Points outside the geometry are ignored.
      void insertPoints(const fileName& pointFileName);
      //- Create the initial mesh as a regular grid of points.
      //  Points outside the geometry are ignored.
      void insertGrid();
      //- Insert all surface point-pairs from
      //  insertSurfaceNearestPointPairs and
      //  findIntersectionForOutsideCentroid
      void insertSurfacePointPairs();
      //- Insert point-pairs where there are protrusions into
      //  or out of the surface
      void boundaryConform();
    // Point removal
      //- Remove the point-pairs introduced by insertSurfacePointPairs
      //  and boundaryConform
      void removeSurfacePointPairs();
    // Point motion
      inline void movePoint(const Vertex_handle& vh, const Point& P);
      //- Move the internal points to the given new locations and update
      //  the triangulation to ensure it is Delaunay
      // void moveInternalPoints(const point2DField& newPoints);
      //- Calculate the displacements to create the new points
      void newPoints();
      //- Extract patch names and sizes.
      void extractPatches
      (
        wordList& patchNames,
        labelList& patchSizes,
        EdgeMap<label>& mapEdgesRegion,
        EdgeMap<label>& indirectPatchEdge
      ) const;
    // Write
      //- Write internal points to .obj file
      void writePoints(const fileName& fName, bool internalOnly) const;
      //- Write triangles as .obj file
      void writeTriangles(const fileName& fName, bool internalOnly) const;
      //- Write dual faces as .obj file
      void writeFaces(const fileName& fName, bool internalOnly) const;
      //- Calculates dual points (circumcentres of tets) and faces
      //  (point-cell walk of tets).
      //  Returns:
      //  - dualPoints (in triangle ordering)
      //  - dualFaces (compacted)
      void calcDual
      (
        point2DField& dualPoints,
        faceList& dualFaces,
        wordList& patchNames,
        labelList& patchSizes,
        EdgeMap<label>& mapEdgesRegion,
        EdgeMap<label>& indirectPatchEdge
      ) const;
      //- Write patch
      void writePatch(const fileName& fName) const;
      void write() const;
};

inline bool boundaryTriangle(const CV2D::Face_handle fc);
inline bool outsideTriangle(const CV2D::Face_handle fc);

}  // namespace mousse


// Private Member Functions 
inline mousse::label mousse::CV2D::insertPoint
(
  const point2D& p,
  const label type
)
{
  unsigned int nVert = number_of_vertices();
  return insertPoint(toPoint(p), nVert, type);
}


inline mousse::label mousse::CV2D::insertPoint
(
  const point2D& p,
  const label index,
  const label type
)
{
  return insertPoint(toPoint(p), index, type);
}


inline mousse::label mousse::CV2D::insertPoint
(
  const Point& p,
  const label index,
  const label type
)
{
  unsigned int nVert = number_of_vertices();
  Vertex_handle vh = insert(p);
  if (nVert == number_of_vertices()) {
    WARNING_IN_FUNCTION
      << "Failed to insert point " << toPoint2D(p) << endl;
  } else {
    vh->index() = index;
    vh->type() = type;
  }
  return vh->index();
}


inline bool mousse::CV2D::insertMirrorPoint
(
  const point2D& nearSurfPt,
  const point2D& surfPt
)
{
  point2D mirrorPoint{2*surfPt - nearSurfPt};
  if (qSurf_.outside(toPoint3D(mirrorPoint))) {
    insertPoint(mirrorPoint, Vb::MIRROR_POINT);
    return true;
  }
  return false;
}


inline void mousse::CV2D::insertPointPair
(
  const scalar ppDist,
  const point2D& surfPt,
  const vector2D& n
)
{
  vector2D ppDistn = ppDist*n;
  label master =
    insertPoint
    (
      surfPt - ppDistn,
      number_of_vertices() + 1
    );
  insertPoint(surfPt + ppDistn, master);
}


// Member Functions 
inline const mousse::cv2DControls& mousse::CV2D::meshControls() const
{
  return controls_;
}


inline const mousse::point2D& mousse::CV2D::toPoint2D(const point& p) const
{
  return reinterpret_cast<const point2D&>(p);
}


inline const mousse::point2DField mousse::CV2D::toPoint2D(const pointField& p) const
{
  point2DField temp{p.size()};
  FOR_ALL(temp, pointI) {
    temp[pointI] = point2D(p[pointI].x(), p[pointI].y());
  }
  return temp;
}


inline mousse::point mousse::CV2D::toPoint3D(const point2D& p) const
{
  return point(p.x(), p.y(), z_);
}


#ifdef CGAL_INEXACT

inline mousse::CV2D::point2DFromPoint mousse::CV2D::toPoint2D(const Point& P) const
{
  return reinterpret_cast<point2DFromPoint>(P);
}


inline mousse::CV2D::PointFromPoint2D mousse::CV2D::toPoint(const point2D& p) const
{
  return reinterpret_cast<PointFromPoint2D>(p);
}

#else

inline mousse::CV2D::point2DFromPoint mousse::CV2D::toPoint2D(const Point& P) const
{
  return point2D(CGAL::to_double(P.x()), CGAL::to_double(P.y()));
}


inline mousse::CV2D::PointFromPoint2D mousse::CV2D::toPoint(const point2D& p) const
{
  return Point(p.x(), p.y());
}

#endif

inline mousse::point mousse::CV2D::toPoint3D(const Point& P) const
{
  return {CGAL::to_double(P.x()), CGAL::to_double(P.y()), z_};
}


inline void mousse::CV2D::movePoint(const Vertex_handle& vh, const Point& P)
{
  int i = vh->index();
  int t = vh->type();
  remove(vh);
  Vertex_handle newVh = insert(P);
  newVh->index() = i;
  newVh->type() = t;
}


// Friend Functions 
inline bool mousse::boundaryTriangle(const CV2D::Face_handle fc)
{
  return
    boundaryTriangle(*fc->vertex(0), *fc->vertex(1), *fc->vertex(2));
}


inline bool mousse::outsideTriangle(const CV2D::Face_handle fc)
{
  return
    outsideTriangle(*fc->vertex(0), *fc->vertex(1), *fc->vertex(2));
}

#endif
