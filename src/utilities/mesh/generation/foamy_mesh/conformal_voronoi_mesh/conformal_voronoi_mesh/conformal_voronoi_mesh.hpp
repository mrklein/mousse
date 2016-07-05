#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::conformalVoronoiMesh

// Include uint.H before CGAL headers to define __STDC_LIMIT_MACROS
#include "uint.hpp"
#include "cgal_triangulation_3d_defs.hpp"
#include "searchable_surfaces.hpp"
#include "conformation_surfaces.hpp"
#include "cell_shape_control.hpp"
#include "cv_controls.hpp"
#include "dynamic_list.hpp"
#include "packed_bool_list.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "plane.hpp"
#include "sortable_list.hpp"
#include "mesh_tools.hpp"
#include "dynamic_indexed_octree.hpp"
#include "dynamic_tree_data_point.hpp"
#include "indexed_octree.hpp"
#include "tree_data_point.hpp"
#include "unit_conversion.hpp"
#include "transform.hpp"
#include "vol_fields.hpp"
#include "fv_mesh.hpp"
#include "label_pair.hpp"
#include "hash_set.hpp"
#include "mem_info.hpp"
#include "point.hpp"
#include "cell_set.hpp"
#include "wall_poly_patch.hpp"
#include "processor_poly_patch.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "global_index.hpp"
#include "point_feature_edges_types.hpp"
#include "point_conversion.hpp"
#include "pair.hpp"
#include "distributed_delaunay_mesh.hpp"
#include "feature_point_conformer.hpp"
#include "point_pairs.hpp"
#include "indexed_vertex_ops.hpp"
#include "indexed_cell_ops.hpp"


namespace mousse {

// Forward declaration of classes
class initialPointsMethod;
class relaxationModel;
class faceAreaWeightModel;
class backgroundMeshDecomposition;
class OBJstream;


class conformalVoronoiMesh
:
  public DistributedDelaunayMesh<Delaunay>
{
public:
  typedef Delaunay::Vertex_handle Vertex_handle;
  typedef Delaunay::Cell_handle Cell_handle;
  typedef Delaunay::Edge Edge;
  typedef Delaunay::Facet Facet;
  typedef Delaunay::Point Point;
  typedef List<DynamicList<Pair<labelPair>>> labelPairPairDynListList;
  typedef Tuple2<pointIndexHit, label> pointIndexHitAndFeature;
  typedef List<pointIndexHitAndFeature> pointIndexHitAndFeatureList;
  typedef DynamicList<pointIndexHitAndFeature> pointIndexHitAndFeatureDynList;
  // Static data
    enum dualMeshPointType
    {
      internal     = 0,
      surface      = 1,
      featureEdge  = 2,
      featurePoint = 3,
      constrained  = 4
    };
    static const NamedEnum<dualMeshPointType, 5> dualMeshPointTypeNames_;
private:
  // Static data
    static const scalar searchConeAngle;
    static const scalar searchAngleOppositeSurface;
  // Private data
    //- The time registry of the application
    const Time& runTime_;
    //- Random number generator
    mutable Random rndGen_;
    //- Controls for the conformal Voronoi meshing process
    cvControls foamyHexMeshControls_;
    //- All geometry of the meshing process, including surfaces to be
    //  conformed to and those to be used for refinement
    searchableSurfaces allGeometry_;
    //- The surfaces to conform to
    conformationSurfaces geometryToConformTo_;
    //- Background mesh decomposition, only available in parallel.
    autoPtr<backgroundMeshDecomposition> decomposition_;
    //- The cell shape control object
    cellShapeControl cellShapeControl_;
    //- Limiting bound box before infinity begins
    treeBoundBox limitBounds_;
    //-
    mutable pointPairs<Delaunay> ptPairs_;
    //-
    featurePointConformer ftPtConformer_;
    //- Search tree for edge point locations
    mutable autoPtr<dynamicIndexedOctree<dynamicTreeDataPoint>>
      edgeLocationTreePtr_;
    mutable DynamicList<mousse::point> existingEdgeLocations_;
    //- Search tree for surface point locations
    mutable autoPtr<dynamicIndexedOctree<dynamicTreeDataPoint>>
      surfacePtLocationTreePtr_;
    mutable DynamicList<mousse::point> existingSurfacePtLocations_;
    //- Store the surface and feature edge conformation locations to be
    //  reinserted
    List<Vb> surfaceConformationVertices_;
    //- Method for inserting initial points.  Runtime selectable.
    autoPtr<initialPointsMethod> initialPointsMethod_;
    //- Relaxation coefficient model.  Runtime selectable.
    autoPtr<relaxationModel> relaxationModel_;
    //-  Face area weight function.  Runtime selectable.
    autoPtr<faceAreaWeightModel> faceAreaWeightModel_;
  // Private Member Functions
    inline scalar defaultCellSize() const;
    //- Return the local target cell size at the given location.  Takes
    //  boolean argument to allow speed-up of queries if the point is going
    //  to be on a surface.
    inline scalar targetCellSize(const mousse::point& pt) const;
    //- Return the target cell size from that stored on a pair of
    //  Delaunay vertices, including the possibility that one of
    //  them is not an internalOrBoundaryPoint, and so will not
    //  have valid data.
    inline scalar averageAnyCellSize
    (
      const Vertex_handle& vA,
      const Vertex_handle& vB
    ) const;
    //- The average target cell size of a Delaunay facet, i.e., of
    //  a dual edge
    inline scalar averageAnyCellSize
    (
      const Delaunay::Finite_facets_iterator& fit
    ) const;
    //- Insert Delaunay vertices using the CGAL range insertion method,
    //  optionally check processor occupancy and distribute to other
    //  processors
    void insertInternalPoints
    (
      List<Point>& points,
      const bool distribute = false
    );
    Map<label> insertPointPairs
    (
      List<Vb>& vertices,
      bool distribute,
      bool reIndex
    );
    //- Create a point-pair at a ppDist distance either side of
    //  surface point surfPt, in the direction n
    inline void createPointPair
    (
      const scalar ppDist,
      const mousse::point& surfPt,
      const vector& n,
      const bool ptPair,
      DynamicList<Vb>& pts
    ) const;
    inline mousse::point perturbPoint(const mousse::point& pt) const;
    //- Create a point-pair at a ppDist distance either side of
    //  surface point surfPt, in the direction n
    inline void createBafflePointPair
    (
      const scalar ppDist,
      const mousse::point& surfPt,
      const vector& n,
      const bool ptPair,
      DynamicList<Vb>& pts
    ) const;
    //- Check internal point is completely inside the meshable region
    inline bool internalPointIsInside(const mousse::point& pt) const;
    //- Insert pairs of points on the surface with the given normals, at the
    //  specified spacing
    void insertSurfacePointPairs
    (
      const pointIndexHitAndFeatureList& surfaceHits,
      const fileName fName,
      DynamicList<Vb>& pts
    );
    //- Insert groups of points to conform to an edge given a list of
    //  pointIndexHits specifying the location and edge index of the point
    //  to be conformed to on the corresponding entry in featureHit
    void insertEdgePointGroups
    (
      const pointIndexHitAndFeatureList& edgeHits,
      const fileName fName,
      DynamicList<Vb>& pts
    );
    void createEdgePointGroupByCirculating
    (
      const extendedFeatureEdgeMesh& feMesh,
      const pointIndexHit& edHit,
      DynamicList<Vb>& pts
    ) const;
    bool meshableRegion
    (
      const plane::side side,
      const extendedFeatureEdgeMesh::sideVolumeType volType
    ) const;
    bool regionIsInside
    (
      const extendedFeatureEdgeMesh::sideVolumeType volTypeA,
      const vector& normalA,
      const extendedFeatureEdgeMesh::sideVolumeType volTypeB,
      const vector& normalB,
      const vector& masterPtVec
    ) const;
    //- Create points to conform to an external edge
    void createExternalEdgePointGroup
    (
      const extendedFeatureEdgeMesh& feMesh,
      const pointIndexHit& edHit,
      DynamicList<Vb>& pts
    ) const;
    //- Create points to conform to an internal edge
    void createInternalEdgePointGroup
    (
      const extendedFeatureEdgeMesh& feMesh,
      const pointIndexHit& edHit,
      DynamicList<Vb>& pts
    ) const;
    //- Create points to conform to a flat edge
    void createFlatEdgePointGroup
    (
      const extendedFeatureEdgeMesh& feMesh,
      const pointIndexHit& edHit,
      DynamicList<Vb>& pts
    ) const;
    //- Create points to conform to an open edge
    void createOpenEdgePointGroup
    (
      const extendedFeatureEdgeMesh& feMesh,
      const pointIndexHit& edHit,
      DynamicList<Vb>& pts
    ) const;
    //- Create points to conform to multiply connected edge
    void createMultipleEdgePointGroup
    (
      const extendedFeatureEdgeMesh& feMesh,
      const pointIndexHit& edHit,
      DynamicList<Vb>& pts
    ) const;
    //- Determine and insert point groups at the feature points
    void insertFeaturePoints(bool distribute = false);
    //- Check if a location is in exclusion range around a feature point
    bool nearFeaturePt(const mousse::point& pt) const;
    //- Check if a surface point is in exclusion range around a feature edge
    bool surfacePtNearFeatureEdge(const mousse::point& pt) const;
    //- Insert the initial points into the triangulation, based on the
    //  initialPointsMethod
    void insertInitialPoints();
    //- In parallel redistribute the backgroundMeshDecomposition and
    //  vertices to balance the number of vertices on each processor.
    //  Returns true if the background mesh changes as this removes all
    //  referred vertices, so the parallel interface may need rebuilt.
    template<class Triangulation>
    bool distributeBackground(const Triangulation& mesh);
    // Test for full containment
    void cellSizeMeshOverlapsBackground() const;
    //-
    void distribute();
    void buildCellSizeAndAlignmentMesh();
    //- Set the size and alignment data for each vertex
    void setVertexSizeAndAlignment();
    //- Builds a dual face by circulating around the supplied edge.
    face buildDualFace
    (
      const Delaunay::Finite_edges_iterator& eit
    ) const;
    boolList dualFaceBoundaryPoints
    (
      const Delaunay::Finite_edges_iterator& eit
    ) const;
    //- Finds the maximum filterCount of the dual vertices
    //  (Delaunay cells) that form the dual face produced by the
    //  supplied edge
    label maxFilterCount
    (
      const Delaunay::Finite_edges_iterator& eit
    ) const;
    //- Determines the owner and neighbour labels for dual cells
    //  corresponding to the dual face formed by the supplied
    //  Delaunay vertices.  If the dual face is a boundary face
    //  then neighbour = -1.  Returns true if the dual face
    //  created by vA -> vB needs to be reversed to be correctly
    //  orientated.
    bool ownerAndNeighbour
    (
      Vertex_handle vA,
      Vertex_handle vB,
      label& owner,
      label& neighbour
    ) const;
    //- Insert the necessary point pairs to conform to the surface, either
    //  from stored results, or trigger a re-conformation
    void conformToSurface();
    //- Decision making function for when to rebuild the surface
    //  conformation
    bool reconformToSurface() const;
    //- Determines geometrically whether a vertex is close to a surface
    //  This is an optimisation
    label findVerticesNearBoundaries();
    //- Create and insert the necessary point pairs to conform to the
    //  surface, then store the result
    void buildSurfaceConformation();
    label synchroniseEdgeTrees
    (
      const DynamicList<label>& edgeToTreeShape,
      pointIndexHitAndFeatureList& featureEdgeHits
    );
    label synchroniseSurfaceTrees
    (
      const DynamicList<label>& surfaceToTreeShape,
      pointIndexHitAndFeatureList& surfaceHits
    );
    bool surfaceLocationConformsToInside
    (
      const pointIndexHitAndFeature& info
    ) const;
    //- Check to see if dual cell specified by given vertex iterator
    //  intersects the boundary and hence reqires a point-pair
    bool dualCellSurfaceAnyIntersection
    (
      const Delaunay::Finite_vertices_iterator& vit
    ) const;
    //- Return all intersections
    bool dualCellSurfaceAllIntersections
    (
      const Delaunay::Finite_vertices_iterator& vit,
      pointIndexHitAndFeatureDynList& info
    ) const;
    //- Return false if the line is entirely outside the current processor
    //  domain, true is either point is inside, or the processor domain
    //  bounadry is intersected (i.e. the points are box outside but the
    //  line cuts.  The points will be moved onto the box where they
    //  intersect.
    bool clipLineToProc
    (
      const mousse::point& pt,
      mousse::point& a,
      mousse::point& b
    ) const;
    //- Find the "worst" protrusion of a dual cell through the surface,
    //  subject to the maxSurfaceProtrusion tolerance
    void dualCellLargestSurfaceProtrusion
    (
      const Delaunay::Finite_vertices_iterator& vit,
      pointIndexHit& surfHit,
      label& hitSurface
    ) const;
    void dualCellLargestSurfaceIncursion
    (
      const Delaunay::Finite_vertices_iterator& vit,
      pointIndexHit& surfHit,
      label& hitSurface
    ) const;
    //- Write out vertex-processor occupancy information for debugging
    void reportProcessorOccupancy();
    //- Write out debugging information about the surface conformation
    //  quality
//        void reportSurfaceConformationQuality();
    //- Limit the displacement of a point so that it doesn't penetrate the
    //  surface to be meshed or come too close to it
    void limitDisplacement
    (
      const Delaunay::Finite_vertices_iterator& vit,
      vector& displacement,
      label callCount = 0
    ) const;
    //- Find angle between the normals of two close surface points.
    scalar angleBetweenSurfacePoints(mousse::point pA, mousse::point pB) const;
    //- Check if a surface point is near another.
    bool nearSurfacePoint
    (
      pointIndexHitAndFeature& pHit
    ) const;
    //- Append a point to the surface point tree and the existing list
    bool appendToSurfacePtTree
    (
      const mousse::point& pt
    ) const;
    //- Append a point to the edge location tree and the existing list
    bool appendToEdgeLocationTree
    (
      const mousse::point& pt
    ) const;
    //- Return a list of the nearest feature edge locations
    List<pointIndexHit> nearestFeatureEdgeLocations
    (
      const mousse::point& pt
    ) const;
    //- Check if a point is near any feature edge points.
    bool pointIsNearFeatureEdgeLocation(const mousse::point& pt) const;
    bool pointIsNearFeatureEdgeLocation
    (
      const mousse::point& pt,
      pointIndexHit& info
    ) const;
    //- Check if a point is near any surface conformation points.
    bool pointIsNearSurfaceLocation(const mousse::point& pt) const;
    bool pointIsNearSurfaceLocation
    (
      const mousse::point& pt,
      pointIndexHit& info
    ) const;
    //- Check if a location is in the exclusion range of an existing feature
    //- Edge conformation location
    bool nearFeatureEdgeLocation
    (
      const pointIndexHit& pHit,
      pointIndexHit& nearestEdgeHit
    ) const;
    //- Build or rebuild the edge location tree
    void buildEdgeLocationTree
    (
      const DynamicList<mousse::point>& existingEdgeLocations
    ) const;
    //- Build or rebuild the surface point location tree
    void buildSurfacePtLocationTree
    (
      const DynamicList<mousse::point>& existingSurfacePtLocations
    ) const;
    //- Process the surface conformation locations to decide which surface
    //  and edge conformation locations to add
    void addSurfaceAndEdgeHits
    (
      const mousse::point& vit,
      const pointIndexHitAndFeatureDynList& surfaceIntersections,
      scalar surfacePtReplaceDistCoeffSqr,
      scalar edgeSearchDistCoeffSqr,
      pointIndexHitAndFeatureDynList& surfaceHits,
      pointIndexHitAndFeatureDynList& featureEdgeHits,
      DynamicList<label>& surfaceToTreeShape,
      DynamicList<label>& edgeToTreeShape,
      Map<scalar>& surfacePtToEdgePtDist,
      bool firstPass
    ) const;
    //- Store the surface conformation with the indices offset to be
    //  relative to zero
    void storeSurfaceConformation();
    //- Reinsert the surface conformation re-offsetting indices to be
    //  relative to new number of internal vertices
    void reinsertSurfaceConformation();
    void checkCells();
    void checkDuals();
    void checkVertices();
    void checkCoPlanarCells() const;
    //- Dual calculation
    void calcDualMesh
    (
      pointField& points,
      labelList& boundaryPts,
      faceList& faces,
      labelList& owner,
      labelList& neighbour,
      wordList& patchNames,
      PtrList<dictionary>& patchDicts,
      pointField& cellCentres,
      labelList& cellToDelaunayVertex,
      labelListList& patchToDelaunayVertex,
      PackedBoolList& boundaryFacesToRemove
    );
    void calcNeighbourCellCentres
    (
      const polyMesh& mesh,
      const pointField& cellCentres,
      pointField& neiCc
    ) const;
    void selectSeparatedCoupledFaces
    (
      const polyMesh& mesh,
      boolList& selected
    ) const;
    //- From meshRefinementBaffles.C. Use insidePoint for a surface to
    //  determine the cell zone.
    void findCellZoneInsideWalk
    (
      const polyMesh& mesh,
      const labelList& locationSurfaces,
      const labelList& faceToSurface,
      labelList& cellToSurface
    ) const;
    //- Calculate the cell zones from cellCentres using all closed surfaces
    labelList calcCellZones(const pointField& cellCentres) const;
    //- Calculate the face zones
    void calcFaceZones
    (
      const polyMesh& mesh,
      const pointField& cellCentres,
      const labelList& cellToSurface,
      labelList& faceToSurface,
      boolList& flipMap
    ) const;
    //- Add zones to the polyMesh
    void addZones(polyMesh& mesh, const pointField& cellCentres) const;
    //- Tet mesh calculation
    void calcTetMesh
    (
      pointField& points,
      labelList& pointToDelaunayVertex,
      faceList& faces,
      labelList& owner,
      labelList& neighbour,
      wordList& patchNames,
      PtrList<dictionary>& patchDicts
    );
    //- Determines if the dual face constructed by the Delaunay
    //  edge is a boundary face
    inline bool isBoundaryDualFace
    (
      const Delaunay::Finite_edges_iterator& eit
    ) const;
    //- Which processors are attached to the dual edge represented by this
    //  Delaunay facet
    inline List<label> processorsAttached
    (
      const Delaunay::Finite_facets_iterator& fit
    ) const;
    //- Determines if the edge constructed from the face is on
    //  a processor patch
    inline bool isParallelDualEdge
    (
      const Delaunay::Finite_facets_iterator& fit
    ) const;
    //- Determines if the dual face constructed by the Delaunay
    //  edge is a processor boundary face
    inline bool isProcBoundaryEdge
    (
      const Delaunay::Finite_edges_iterator& eit
    ) const;
    //- Merge vertices that are identical
    void mergeIdenticalDualVertices
    (
      const pointField& pts,
      labelList& boundaryPts
    );
    label mergeIdenticalDualVertices
    (
      const pointField& pts,
      Map<label>& dualPtIndexMap
    ) const;
    //- Identify the face labels of the deferred collapse faces
    void deferredCollapseFaceSet
    (
      labelList& owner,
      labelList& neighbour,
      const HashSet<labelPair, labelPair::Hash<>>& deferredCollapseFaces
    ) const;
    //- Check whether the cell sizes are fine enough. Creates a polyMesh.
    void checkCellSizing();
    //- Find all cells with a patch face that is not near the surface. The
    //  allowed offset is the fraction of the target cell size.
    labelHashSet findOffsetPatchFaces
    (
      const polyMesh& mesh,
      const scalar allowedOffset
    ) const;
    //- Create a polyMesh and check its quality, reports which
    //  elements damage the mesh quality, allowing backtracking.
    labelHashSet checkPolyMeshQuality(const pointField& pts) const;
    label classifyBoundaryPoint(Cell_handle cit) const;
    //- Index all of the the Delaunay cells and calculate their
    //- Dual points
    void indexDualVertices
    (
      pointField& pts,
      labelList& boundaryPts
    );
    //- Re-index all of the Delaunay cells
    void reindexDualVertices
    (
      const Map<label>& dualPtIndexMap,
      labelList& boundaryPts
    );
    label createPatchInfo
    (
      wordList& patchNames,
      PtrList<dictionary>& patchDicts
    ) const;
    vector calcSharedPatchNormal(Cell_handle c1, Cell_handle c2) const;
    bool boundaryDualFace(Cell_handle c1, Cell_handle c2) const;
    //- Create all of the internal and boundary faces
    void createFacesOwnerNeighbourAndPatches
    (
      const pointField& pts,
      faceList& faces,
      labelList& owner,
      labelList& neighbour,
      wordList& patchNames,
      PtrList<dictionary>& patchDicts,
      labelListList& patchPointPairSlaves,
      PackedBoolList& boundaryFacesToRemove,
      bool includeEmptyPatches = false
    ) const;
    //- Sort the faces, owner and neighbour lists into
    //  upper-triangular order.  For internal faces only, use
    //  before adding patch faces
    void sortFaces
    (
      faceList& faces,
      labelList& owner,
      labelList& neighbour
    ) const;
    //- Sort the processor patches so that the faces are in the same order
    //  on both processors
    void sortProcPatches
    (
      List<DynamicList<face>>& patchFaces,
      List<DynamicList<label>>& patchOwners,
      List<DynamicList<label>>& patchPointPairSlaves,
      labelPairPairDynListList& patchSortingIndices
    ) const;
    //- Add the faces and owner information for the patches
    void addPatches
    (
      const label nInternalFaces,
      faceList& faces,
      labelList& owner,
      PtrList<dictionary>& patchDicts,
      PackedBoolList& boundaryFacesToRemove,
      const List<DynamicList<face>>& patchFaces,
      const List<DynamicList<label>>& patchOwners,
      const List<DynamicList<bool>>& indirectPatchFace
    ) const;
    //- Remove points that are no longer used by any faces
    void removeUnusedPoints
    (
      faceList& faces,
      pointField& pts,
      labelList& boundaryPts
    ) const;
    //- Remove dual cells that are not used by any faces. Return compaction
    //  map.
    labelList removeUnusedCells
    (
      labelList& owner,
      labelList& neighbour
    ) const;
    //- Create an empty fvMesh
    autoPtr<fvMesh> createDummyMesh
    (
      const IOobject& io,
      const wordList& patchNames,
      const PtrList<dictionary>& patchDicts
    ) const;
    //- Create a polyMesh from points.
    autoPtr<polyMesh> createPolyMeshFromPoints(const pointField& pts) const;
    void checkProcessorPatchesMatch
    (
      const PtrList<dictionary>& patchDicts
    ) const;
    void reorderPoints
    (
      pointField& points,
      labelList& boundaryPts,
      faceList& faces,
      const label nInternalFaces
    ) const;
    //- Rotate the faces on processor patches if necessary
    void reorderProcessorPatches
    (
      const word& meshName,
      const fileName& instance,
      const pointField& points,
      faceList& faces,
      const wordList& patchNames,
      const PtrList<dictionary>& patchDicts
    ) const;
    void writePointPairs(const fileName& fName) const;
public:
  //- Runtime type information
  CLASS_NAME("conformalVoronoiMesh");
  // Constructors
    //- Construct from Time and foamyHexMeshDict
    conformalVoronoiMesh
    (
      const Time& runTime,
      const dictionary& foamyHexMeshDict
    );
    //- Disallow default bitwise copy construct
    conformalVoronoiMesh(const conformalVoronoiMesh&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const conformalVoronoiMesh&) = delete;
  //- Destructor
  ~conformalVoronoiMesh();
  // Member Functions
    void initialiseForMotion();
    void initialiseForConformation();
    //- Move the vertices according to the controller, re-conforming to the
    //  surface as required
    void move();
    // Access
      //- Return the Time object
      inline const Time& time() const;
      //- Return the random number generator
      inline Random& rndGen() const;
      //- Return the allGeometry object
      inline const searchableSurfaces& allGeometry() const;
      //- Return the conformationSurfaces object
      inline const conformationSurfaces& geometryToConformTo() const;
      //- Return the backgroundMeshDecomposition
      inline const backgroundMeshDecomposition& decomposition() const;
      //- Return the cellShapeControl object
      inline const cellShapeControl& cellShapeControls() const;
      //- Return the foamyHexMeshControls object
      inline const cvControls& foamyHexMeshControls() const;
    // Query
      //- Return the local point pair separation at the given location
      inline scalar pointPairDistance(const mousse::point& pt) const;
      //- Return the local mixed feature point placement distance
      inline scalar mixedFeaturePointDistance
      (
        const mousse::point& pt
      ) const;
      //- Return the square of the local feature point exclusion distance
      inline scalar featurePointExclusionDistanceSqr
      (
        const mousse::point& pt
      ) const;
      //- Return the square of the local feature edge exclusion distance
      inline scalar featureEdgeExclusionDistanceSqr
      (
        const mousse::point& pt
      ) const;
      //- Return the square of the local surface point exclusion distance
      inline scalar surfacePtExclusionDistanceSqr
      (
        const mousse::point& pt
      ) const;
      //- Return the square of the local surface search distance
      inline scalar surfaceSearchDistanceSqr(const mousse::point& pt) const;
      //- Return the local maximum surface protrusion distance
      inline scalar maxSurfaceProtrusion(const mousse::point& pt) const;
      //- Call the appropriate function to conform to an edge
      void createEdgePointGroup
      (
        const extendedFeatureEdgeMesh& feMesh,
        const pointIndexHit& edHit,
        DynamicList<Vb>& pts
      ) const;
    // Write
      //- Write the elapsedCpuTime and memory usage, with an optional
      //  description
      static void timeCheck
      (
        const Time& runTime,
        const string& description = string::null,
        const bool check = true
      );
      void timeCheck
      (
        const string& description = string::null
      ) const;
      //- Prepare data and call writeMesh for polyMesh and
      //  tetDualMesh
      void writeMesh(const fileName& instance);
      //- Write mesh to disk
      void writeMesh
      (
        const word& meshName,
        const fileName& instance,
        pointField& points,
        labelList& boundaryPts,
        faceList& faces,
        labelList& owner,
        labelList& neighbour,
        const wordList& patchNames,
        const PtrList<dictionary>& patchDicts,
        const pointField& cellCentres,
        PackedBoolList& boundaryFacesToRemove
      ) const;
      //- Calculate and write a field of the target cell size,
      //  target cell volume, actual cell volume and equivalent
      //  actual cell size (cbrt(actual cell volume)).
      void writeCellSizes(const fvMesh& mesh) const;
      void writeCellAlignments(const fvMesh& mesh) const;
      //- Calculate and write the cell centres.
      void writeCellCentres(const fvMesh& mesh) const;
      //- Find the cellSet of the boundary cells which have points that
      //  protrude out of the surface beyond a tolerance.
      labelHashSet findRemainingProtrusionSet(const polyMesh& mesh) const;
};

}  // namespace mousse


// Private Member Functions 
inline mousse::scalar mousse::conformalVoronoiMesh::defaultCellSize() const
{
  return foamyHexMeshControls().defaultCellSize();
}


inline mousse::scalar mousse::conformalVoronoiMesh::targetCellSize
(
  const mousse::point& pt
) const
{
  return cellShapeControls().cellSize(pt);
}


inline mousse::scalar mousse::conformalVoronoiMesh::averageAnyCellSize
(
  const Vertex_handle& vA,
  const Vertex_handle& vB
) const
{
  if ((!vA->internalOrBoundaryPoint() || vA->referred())
      && (!vB->internalOrBoundaryPoint() || vB->referred())) {
    // There are no internalOrBoundaryPoints available, determine
    // size from scratch
    // Geometric mean
    const scalar vAsize = targetCellSize(topoint(vA->point()));
    const scalar vBsize = targetCellSize(topoint(vB->point()));
    return sqrt(vAsize*vBsize);
  } else if (!vB->internalOrBoundaryPoint() || vB->referred()) {
    return vA->targetCellSize();
  } else if (!vA->internalOrBoundaryPoint() || vA->referred()) {
    return vB->targetCellSize();
  }
  return CGAL::indexedVertexOps::averageCellSize(vA, vB);
}


inline mousse::scalar mousse::conformalVoronoiMesh::averageAnyCellSize
(
  const Delaunay::Finite_facets_iterator& fit
) const
{
  // Arithmetic mean
  scalar sizeSum = 0;
  label nProducts = 0;
  const Cell_handle c{fit->first};
  const label oppositeVertex = fit->second;
  for (label i = 0; i < 3; i++) {
    Vertex_handle v = c->vertex(vertex_triple_index(oppositeVertex, i));
    if (v->internalOrBoundaryPoint() && !v->referred()) {
      sizeSum += v->targetCellSize();
      nProducts++;
    }
  }
  if (nProducts < 1) {
    // There are no internalOrBoundaryPoints available, determine
    // size from scratch
    for (label i = 0; i < 3; i++) {
      Vertex_handle v = c->vertex(vertex_triple_index(oppositeVertex, i));
      sizeSum += targetCellSize(topoint(v->point()));
    }
    nProducts = 3;
  }
  if (sizeSum < 0) {
    WARNING_IN("averageAnyCellSize(const Delaunay::Finite_facets_iterator&)")
      << "sizeSum = " << sizeSum
      << endl;
    return 0;
  }
  return pow(sizeSum, (1.0/nProducts));
}


inline mousse::scalar mousse::conformalVoronoiMesh::pointPairDistance
(
  const mousse::point& pt
) const
{
  return targetCellSize(pt)*foamyHexMeshControls().pointPairDistanceCoeff();
}


inline mousse::scalar mousse::conformalVoronoiMesh::mixedFeaturePointDistance
(
  const mousse::point& pt
) const
{
  const auto& ctls = foamyHexMeshControls();
  return
    pointPairDistance(pt)*ctls.mixedFeaturePointPPDistanceCoeff();
}


inline mousse::scalar mousse::conformalVoronoiMesh::featurePointExclusionDistanceSqr
(
  const mousse::point& pt
) const
{
  const auto& ctls = foamyHexMeshControls();
  return sqr(targetCellSize(pt)*ctls.featurePointExclusionDistanceCoeff());
}


inline mousse::scalar mousse::conformalVoronoiMesh::featureEdgeExclusionDistanceSqr
(
  const mousse::point& pt
) const
{
  const auto& ctls = foamyHexMeshControls();
  return sqr(targetCellSize(pt)*ctls.featureEdgeExclusionDistanceCoeff());
}


inline mousse::scalar mousse::conformalVoronoiMesh::surfacePtExclusionDistanceSqr
(
  const mousse::point& pt
) const
{
  const auto& ctls = foamyHexMeshControls();
  return sqr(targetCellSize(pt)*ctls.surfacePtExclusionDistanceCoeff());
}


inline mousse::scalar mousse::conformalVoronoiMesh::surfaceSearchDistanceSqr
(
  const mousse::point& pt
) const
{
  const auto& ctls = foamyHexMeshControls();
  return sqr(targetCellSize(pt)*ctls.surfaceSearchDistanceCoeff());
}


inline mousse::scalar mousse::conformalVoronoiMesh::maxSurfaceProtrusion
(
  const mousse::point& pt
) const
{
  const auto& ctls = foamyHexMeshControls();
  return targetCellSize(pt)*ctls.maxSurfaceProtrusionCoeff();
}


inline void mousse::conformalVoronoiMesh::createPointPair
(
  const scalar ppDist,
  const mousse::point& surfPt,
  const vector& n,
  const bool ptPair,
  DynamicList<Vb>& pts
) const
{
  vector ppDistn = ppDist*n;
  pts.append
  (
    Vb
    {
      surfPt - ppDistn,
      vertexCount() + pts.size(),
      Vb::vtInternalSurface,
      Pstream::myProcNo()
    }
  );
  pts.append
  (
    Vb
    {
      surfPt + ppDistn,
      vertexCount() + pts.size(),
      Vb::vtExternalSurface,
      Pstream::myProcNo()
    }
  );
  if (ptPair) {
    ptPairs_.addPointPair
    (
      pts[pts.size() - 2].index(),
      pts[pts.size() - 1].index() // external 0 -> slave
    );
  }
}


inline mousse::point mousse::conformalVoronoiMesh::perturbPoint
(
  const mousse::point& pt
) const
{
  mousse::point perturbedPt(pt);
  scalar pert = 1e-12*defaultCellSize();
  perturbedPt.x() += pert*(rndGen_.scalar01() - 0.5);
  perturbedPt.y() += pert*(rndGen_.scalar01() - 0.5);
  perturbedPt.z() += pert*(rndGen_.scalar01() - 0.5);
  return perturbedPt;
}


inline void mousse::conformalVoronoiMesh::createBafflePointPair
(
  const scalar ppDist,
  const mousse::point& surfPt,
  const vector& n,
  const bool ptPair,
  DynamicList<Vb>& pts
) const
{
  vector ppDistn = ppDist*n;
  pts.append
  (
    Vb
    {
      surfPt - ppDistn,
      vertexCount() + pts.size(),
      Vb::vtInternalSurfaceBaffle,
      Pstream::myProcNo()
    }
  );
  pts.append
  (
    Vb
    {
      surfPt + ppDistn,
      vertexCount() + pts.size(),
      Vb::vtExternalSurfaceBaffle,
      Pstream::myProcNo()
    }
  );
  if (ptPair) {
    ptPairs_.addPointPair
    (
      pts[pts.size() - 2].index(), // external 0 -> slave
      pts[pts.size() - 1].index()
    );
  }
}


inline bool mousse::conformalVoronoiMesh::internalPointIsInside
(
  const mousse::point& pt
) const
{
  if (!geometryToConformTo_.globalBounds().contains(pt)
      || !geometryToConformTo_.inside(pt)) {
    return false;
  }
  return true;
}


inline bool mousse::conformalVoronoiMesh::isBoundaryDualFace
(
  const Delaunay::Finite_edges_iterator& eit
) const
{
  Cell_handle c = eit->first;
  Vertex_handle vA = c->vertex(eit->second);
  Vertex_handle vB = c->vertex(eit->third);
  // A dual face on the boundary will result from one Dv inside and
  // one outside
  return (((vA->internalOrBoundaryPoint() && !vA->referred())
           || (vB->internalOrBoundaryPoint() && !vB->referred()))
          && (!vA->internalOrBoundaryPoint()
              || !vB->internalOrBoundaryPoint()));
}


inline mousse::List<bool> mousse::conformalVoronoiMesh::dualFaceBoundaryPoints
(
  const Delaunay::Finite_edges_iterator& eit
) const
{
  Cell_circulator ccStart = incident_cells(*eit);
  Cell_circulator cc1 = ccStart;
  Cell_circulator cc2 = cc1;
  // Advance the second circulator so that it always stays on the next
  // cell around the edge;
  cc2++;
  DynamicList<bool> tmpFaceBoundaryPoints;
  do {
    label cc1I = cc1->cellIndex();
    label cc2I = cc2->cellIndex();
    if (cc1I != cc2I) {
      tmpFaceBoundaryPoints.append(cc1->boundaryDualVertex());
    }
    cc1++;
    cc2++;
  } while (cc1 != ccStart);
  return tmpFaceBoundaryPoints;
}


inline mousse::List<mousse::label> mousse::conformalVoronoiMesh::processorsAttached
(
  const Delaunay::Finite_facets_iterator& fit
) const
{
  DynamicList<label> procsAttached{8};
  const Cell_handle c1{fit->first};
  const label oppositeVertex = fit->second;
  const Cell_handle c2{c1->neighbor(oppositeVertex)};
  FixedList<label, 4> c1Procs{CGAL::indexedCellOps::processorsAttached(c1)};
  FixedList<label, 4> c2Procs{CGAL::indexedCellOps::processorsAttached(c2)};
  FOR_ALL(c1Procs, aPI) {
    if (findIndex(procsAttached, c1Procs[aPI] == -1)) {
      procsAttached.append(c1Procs[aPI]);
    }
    if (findIndex(procsAttached, c2Procs[aPI] == -1)) {
      procsAttached.append(c2Procs[aPI]);
    }
  }
  return List<label>{procsAttached};
}


inline bool mousse::conformalVoronoiMesh::isParallelDualEdge
(
  const Delaunay::Finite_facets_iterator& fit
) const
{
  const Cell_handle c1(fit->first);
  const label oppositeVertex = fit->second;
  return (c1->vertex(vertex_triple_index(oppositeVertex, 0))->referred()
          || c1->vertex(vertex_triple_index(oppositeVertex, 1))->referred()
          || c1->vertex(vertex_triple_index(oppositeVertex, 2))->referred());
}


inline bool mousse::conformalVoronoiMesh::isProcBoundaryEdge
(
  const Delaunay::Finite_edges_iterator& eit
) const
{
  bool isProcBoundaryEdge = false;
  Cell_handle c = eit->first;
  Vertex_handle vA = c->vertex(eit->second);
  Vertex_handle vB = c->vertex(eit->third);
  if (((vA->referred() && !vB->referred())
       || (vB->referred() && !vA->referred()))
      && vA->internalOrBoundaryPoint()
      && vB->internalOrBoundaryPoint()) {
    isProcBoundaryEdge = true;
  }
  return isProcBoundaryEdge;
}


// Member Functions 
inline const mousse::Time& mousse::conformalVoronoiMesh::time() const
{
  return runTime_;
}


inline mousse::Random& mousse::conformalVoronoiMesh::rndGen() const
{
  return rndGen_;
}


inline const mousse::searchableSurfaces&
mousse::conformalVoronoiMesh::allGeometry() const
{
  return allGeometry_;
}


inline const mousse::conformationSurfaces&
mousse::conformalVoronoiMesh::geometryToConformTo() const
{
  return geometryToConformTo_;
}


inline const mousse::backgroundMeshDecomposition&
mousse::conformalVoronoiMesh::decomposition() const
{
  if (!Pstream::parRun()) {
    FATAL_ERROR_IN
    (
      "inline const mousse::backgroundMeshDecomposition& "
      "mousse::conformalVoronoiMesh::decomposition() const"
    )
    << "The backgroundMeshDecomposition cannot be asked for in serial."
    << exit(FatalError) << endl;
  }
  return decomposition_();
}


inline const mousse::cellShapeControl&
mousse::conformalVoronoiMesh::cellShapeControls() const
{
  return cellShapeControl_;
}


inline const mousse::cvControls&
mousse::conformalVoronoiMesh::foamyHexMeshControls() const
{
  return foamyHexMeshControls_;
}

#include "conformal_voronoi_mesh.ipp"

#endif
