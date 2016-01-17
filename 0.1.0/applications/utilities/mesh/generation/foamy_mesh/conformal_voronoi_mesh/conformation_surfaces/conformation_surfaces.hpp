// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::conformationSurfaces
// Description
// SourceFiles
//   conformation_surfaces_i.hpp
//   conformation_surfaces.cpp
#ifndef conformation_surfaces_hpp_
#define conformation_surfaces_hpp_
#include "searchable_surfaces.hpp"
#include "searchable_surfaces_queries.hpp"
#include "extended_feature_edge_mesh.hpp"
#include "bool_list.hpp"
#include "volume_type.hpp"
#include "surface_zones_info.hpp"
namespace mousse
{
class conformationSurfaces
{
  typedef extendedFeatureEdgeMesh::sideVolumeType sideVolumeType;
  // Private data
    const Time& runTime_;
    Random& rndGen_;
    //- Reference to the searchableSurfaces object holding all geometry data
    const searchableSurfaces& allGeometry_;
    //- A list of extendedFeatureEdgeMesh
    PtrList<extendedFeatureEdgeMesh> features_;
    //- The location in the mesh that specifies which portion of surfaces is
    //  to be meshed.
    point locationInMesh_;
    //- Indices of surfaces in allGeometry that are to be conformed to
    labelList surfaces_;
    //- Reverse mapping, which entry in surfaces corresponds to the surface
    //  in allGeometry specified by the list index.  -1 for a surface that
    //  isn't used.
    labelList allGeometryToSurfaces_;
    //- A boolean value for each surface to be conformed to specifying if it
    //  is to be treated as a baffle
    List<sideVolumeType> normalVolumeTypes_;
    //- A flat list of all of the names of the patches from all of the
    //  surfaces to be reproduced in the meshed geometry
    List<word> patchNames_;
    //- List of surface zone (face and cell zone) information
    PtrList<surfaceZonesInfo> surfZones_;
    //- The offset between the patch indices of the individual surface and
    //  the entry in the overall patch list
    labelList regionOffset_;
    //- From global region number to patchType
    PtrList<dictionary> patchInfo_;
    //- The overall boundBox of all of the surfaces to be conformed to
    treeBoundBox globalBounds_;
    //- The pattern/signature of volumeTypes representing a point in the
    // domain to be meshed
    List<volumeType> referenceVolumeTypes_;
  // Private Member Functions
    void hasBoundedVolume(List<volumeType>& referenceVolumeTypes) const;
    //- Read into features_ from a dictionary
    void readFeatures
    (
      const label surfI,
      const dictionary& featureDict,
      const word& surfaceName,
      label& featureIndex
    );
    void readFeatures
    (
      const dictionary& featureDict,
      const word& surfaceName,
      label& featureIndex
    );
    //- Disallow default bitwise copy construct
    conformationSurfaces(const conformationSurfaces&);
    //- Disallow default bitwise assignment
    void operator=(const conformationSurfaces&);
public:
  //- Runtime type information
  ClassName("conformationSurfaces");
  // Constructors
    //- Construct from dictionary and references to conformalVoronoiMesh and
    //  searchableSurfaces
    conformationSurfaces
    (
      const Time& runTime,
      Random& rndGen,
      const searchableSurfaces& allGeometry,
      const dictionary& surfaceConformationDict
    );
  //- Destructor
  ~conformationSurfaces();
  // Member Functions
    // Access
      //- Return reference to the searchableSurfaces object containing all
      //  of the geometry
      inline const searchableSurfaces& geometry() const;
      //- Return the object holding the feature points and edges
      inline const PtrList<extendedFeatureEdgeMesh>& features() const;
      //- Return the location to mesh
      inline const point& locationInMesh() const;
      //- Return the surface indices
      inline const labelList& surfaces() const;
      //- Return the patch names
      inline const List<word>& patchNames() const;
      //- Return the surfaceZonesInfo
      inline const PtrList<surfaceZonesInfo>& surfZones() const;
      //- Return the patch info
      inline const PtrList<dictionary>& patchInfo() const;
      //- Return the global bounds
      inline const treeBoundBox& globalBounds() const;
    // Query
      //- Check if the supplied bound box overlaps any part of any of
      //  the surfaces
      bool overlaps(const treeBoundBox& bb) const;
      //- Check if points are inside surfaces to conform to
      Field<bool> inside(const pointField& samplePts) const;
      //- Check if point is inside surfaces to conform to
      bool inside(const point& samplePt) const;
      //- Check if points are outside surfaces to conform to
      Field<bool> outside(const pointField& samplePts) const;
      //- Check if point is outside surfaces to conform to
      bool outside(const point& samplePt) const;
      //- Check if point is closer to the surfaces to conform to than
      //  testDistSqr, in which case return false, otherwise assess in or
      //  outside and return a result depending on the testForInside flag
      Field<bool> wellInOutSide
      (
        const pointField& samplePts,
        const scalarField& testDistSqr,
        bool testForInside
      ) const;
      //- Check if point is inside surfaces to conform to by at least
      //  testDistSqr
      Field<bool> wellInside
      (
        const pointField& samplePts,
        const scalarField& testDistSqr
      ) const;
      bool wellInside
      (
        const point& samplePt,
        scalar testDistSqr
      ) const;
      //- Check if point is outside surfaces to conform to by at least
      //  testDistSqr
      Field<bool> wellOutside
      (
        const pointField& samplePts,
        const scalarField& testDistSqr
      ) const;
      bool wellOutside
      (
        const point& samplePt,
        scalar testDistSqr
      ) const;
      // Finding if the line joining start and end intersects the surface
      bool findSurfaceAnyIntersection
      (
        const point& start,
        const point& end
      ) const;
      //- Finding if the line joining start and end intersects the surface
      //  and returning the hit and surface information
      void findSurfaceAnyIntersection
      (
        const point& start,
        const point& end,
        pointIndexHit& surfHit,
        label& hitSurface
      ) const;
      void findSurfaceAllIntersections
      (
        const point& start,
        const point& end,
        List<pointIndexHit>& surfHit,
        labelList& hitSurface
      ) const;
      //- Finding the nearestIntersection of the surface to start
      void findSurfaceNearestIntersection
      (
        const point& start,
        const point& end,
        pointIndexHit& surfHit,
        label& hitSurface
      ) const;
      //- Find the nearest point to the sample and return it to the
      //  pointIndexHit
      void findSurfaceNearest
      (
        const point& sample,
        scalar nearestDistSqr,
        pointIndexHit& surfHit,
        label& hitSurface
      ) const;
      void findSurfaceNearest
      (
        const pointField& samples,
        const scalarField& nearestDistSqr,
        List<pointIndexHit>& hitInfo,
        labelList& hitSurfaces
      ) const;
      //- Find the nearest point on any feature edge
      void findFeaturePointNearest
      (
        const point& sample,
        scalar nearestDistSqr,
        pointIndexHit& fpHit,
        label& featureHit
      ) const;
      //- Find the nearest point on any feature edge
      void findEdgeNearest
      (
        const point& sample,
        scalar nearestDistSqr,
        pointIndexHit& edgeHit,
        label& featureHit
      ) const;
      void findEdgeNearest
      (
        const pointField& samples,
        const scalarField& nearestDistsSqr,
        List<pointIndexHit>& edgeHits,
        labelList& featuresHit
      ) const;
      //- Find the nearest point on each type of feature edge
      void findEdgeNearestByType
      (
        const point& sample,
        scalar nearestDistSqr,
        List<pointIndexHit>& edgeHit,
        List<label>& featuresHit
      ) const;
      //- Find the nearest points on each feature edge that is within
      //  a given distance from the sample point. Will need to check for
      //  a hit or a miss because near edges may not have a nearest point
      //  on them which is perpendicular to the sample point.
      void findAllNearestEdges
      (
        const point& sample,
        const scalar searchRadiusSqr,
        List<List<pointIndexHit> >& edgeHitsByFeature,
        List<label>& featuresHit
      ) const;
      //- Get the region number of a hit surface
      label getPatchID
      (
        const label hitSurface,
        const pointIndexHit& surfHit
      ) const;
      //- Find which patch is intersected by the line from one point to
      //  another
      label findPatch(const point& ptA, const point& ptB) const;
      //- Find which patch is closest to the point
      label findPatch(const point& pt) const;
      //- Is the surface a baffle.
      extendedFeatureEdgeMesh::sideVolumeType meshableSide
      (
        const label hitSurface,
        const pointIndexHit& surfHit
      ) const;
      void getNormal
      (
        const label hitSurface,
        const List<pointIndexHit>& surfHit,
        vectorField& normal
      ) const;
    // Write
      //- Write all components of all the extendedFeatureEdgeMeshes as
      //  an obj file
      void writeFeatureObj(const fileName& prefix) const;
};
}  // namespace mousse
#include "conformation_surfaces_i.hpp"
#endif
