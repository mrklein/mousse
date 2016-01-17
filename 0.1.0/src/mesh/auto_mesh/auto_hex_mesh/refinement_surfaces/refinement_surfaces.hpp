// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::refinementSurfaces
// Description
//   Container for data on surfaces used for surface-driven refinement.
//   Contains all the data about the level of refinement needed per
//   surface.
// SourceFiles
//   refinement_surfaces.cpp
#ifndef refinement_surfaces_hpp_
#define refinement_surfaces_hpp_
#include "tri_surface_geo_mesh.hpp"
#include "tri_surface_fields.hpp"
#include "vector_list.hpp"
#include "point_index_hit.hpp"
#include "surface_zones_info.hpp"
namespace mousse
{
class searchableSurfaces;
class shellSurfaces;
class triSurfaceMesh;
typedef List<point> pointList;
class refinementSurfaces
{
  // Private data
    //- Reference to all geometry.
    const searchableSurfaces& allGeometry_;
    //- Indices of surfaces that are refinement ones
    labelList surfaces_;
    //- Surface name (word)
    wordList names_;
    //- List of surface zone (face and cell zone) information
    PtrList<surfaceZonesInfo> surfZones_;
    //- From local region number to global region number
    labelList regionOffset_;
    //- From global region number to refinement level
    labelList minLevel_;
    //- From global region number to refinement level
    labelList maxLevel_;
    //- From global region number to small-gap level
    labelList gapLevel_;
    //- From global region number to perpendicular angle
    scalarField perpendicularAngle_;
    //- From global region number to patchType
    PtrList<dictionary> patchInfo_;
public:
  // Constructors
    //- Construct from surfaces and dictionary
    refinementSurfaces
    (
      const searchableSurfaces& allGeometry,
      const dictionary&,
      const label gapLevelIncrement
    );
    //- Construct from components
    refinementSurfaces
    (
      const searchableSurfaces& allGeometry,
      const labelList& surfaces,
      const wordList& names,
      const PtrList<surfaceZonesInfo>& surfZones,
      const labelList& regionOffset,
      const labelList& minLevel,
      const labelList& maxLevel,
      const labelList& gapLevel,
      const scalarField& perpendicularAngle,
      PtrList<dictionary>& patchInfo
    );
    //- Disallow default bitwise copy construct
    refinementSurfaces(const refinementSurfaces&) = delete;
    //- Disallow default bitwise assignment
    refinementSurfaces& operator=(const refinementSurfaces&) = delete;
  // Member Functions
    // Access
      const searchableSurfaces& geometry() const
      {
        return allGeometry_;
      }
      const labelList& surfaces() const
      {
        return surfaces_;
      }
      //- Names of surfaces
      const wordList& names() const
      {
        return names_;
      }
      const PtrList<surfaceZonesInfo>& surfZones() const
      {
        return surfZones_;
      }
      //- From local region number to global region number
      const labelList& regionOffset() const
      {
        return regionOffset_;
      }
      //- From global region number to refinement level
      const labelList& minLevel() const
      {
        return minLevel_;
      }
      //- From global region number to refinement level
      const labelList& maxLevel() const
      {
        return maxLevel_;
      }
      //- From global region number to small gap refinement level
      const labelList& gapLevel() const
      {
        return gapLevel_;
      }
      //- From global region number to perpendicular angle
      const scalarField& perpendicularAngle() const
      {
        return perpendicularAngle_;
      }
      //- From global region number to patch type
      const PtrList<dictionary>& patchInfo() const
      {
        return patchInfo_;
      }
    // Helper
      //- From surface and region on surface to global region
      label globalRegion(const label surfI, const label regionI) const
      {
        return regionOffset_[surfI]+regionI;
      }
      //- Min level for surface and region on surface
      label minLevel(const label surfI, const label regionI) const
      {
        return minLevel_[globalRegion(surfI, regionI)];
      }
      //- Max level for surface and region on surface
      label maxLevel(const label surfI, const label regionI) const
      {
        return maxLevel_[globalRegion(surfI, regionI)];
      }
      label nRegions() const
      {
        return minLevel_.size();
      }
      //- Calculate minLevelFields
      void setMinLevelFields
      (
        const shellSurfaces& shells
      );
      ////- Helper: count number of triangles per region
      //static labelList countRegions(const triSurface&);
    // Searching
      //- Find intersection of edge. Return -1 or first surface
      //  with higher (than currentLevel) minlevel.
      //  Return surface number and level.
      void findHigherIntersection
      (
        const pointField& start,
        const pointField& end,
        const labelList& currentLevel,  // current cell refinement level
        labelList& surfaces,
        labelList& surfaceLevel
      ) const;
      //- Find all intersections of edge. Unsorted order.
      void findAllHigherIntersections
      (
        const pointField& start,
        const pointField& end,
        const labelList& currentLevel,  // current cell refinement level
        const labelList& globalRegionLevel, // level per surfregion
        List<vectorList>& surfaceNormal,
        labelListList& surfaceLevel
      ) const;
      //- Find all intersections of edge. Unsorted order.
      void findAllHigherIntersections
      (
        const pointField& start,
        const pointField& end,
        const labelList& currentLevel,  // current cell refinement level
        const labelList& globalRegionLevel, // level per surfregion
        List<pointList>& surfaceLocation,
        List<vectorList>& surfaceNormal,
        labelListList& surfaceLevel
      ) const;
      //- Find intersection nearest to the endpoints. surface1,2 are
      //  not indices into surfacesToTest but refinement surface indices.
      //  Returns surface, region on surface (so not global surface)
      //  and position on surface.
      void findNearestIntersection
      (
        const labelList& surfacesToTest,
        const pointField& start,
        const pointField& end,
        labelList& surface1,
        List<pointIndexHit>& hit1,
        labelList& region1,
        labelList& surface2,
        List<pointIndexHit>& hit2,
        labelList& region2
      ) const;
      //- findNearestIntersection but also get normals
      void findNearestIntersection
      (
        const labelList& surfacesToTest,
        const pointField& start,
        const pointField& end,
        labelList& surface1,
        List<pointIndexHit>& hit1,
        labelList& region1,
        vectorField& normal1,
        labelList& surface2,
        List<pointIndexHit>& hit2,
        labelList& region2,
        vectorField& normal2
      ) const;
      //- Used for debugging only: find intersection of edge.
      void findAnyIntersection
      (
        const pointField& start,
        const pointField& end,
        labelList& surfaces,
        List<pointIndexHit>&
      ) const;
      //- Find nearest point on surfaces.
      void findNearest
      (
        const labelList& surfacesToTest,
        const pointField& samples,
        const scalarField& nearestDistSqr,
        labelList& surfaces,
        List<pointIndexHit>&
      ) const;
      //- Find nearest point on surfaces. Return surface and region on
      //  surface (so not global surface)
      void findNearestRegion
      (
        const labelList& surfacesToTest,
        const pointField& samples,
        const scalarField& nearestDistSqr,
        labelList& hitSurface,
        labelList& hitRegion
      ) const;
      //- Find nearest point on surfaces. Return surface, region and
      //  normal on surface (so not global surface)
      void findNearestRegion
      (
        const labelList& surfacesToTest,
        const pointField& samples,
        const scalarField& nearestDistSqr,
        labelList& hitSurface,
        List<pointIndexHit>& hitInfo,
        labelList& hitRegion,
        vectorField& hitNormal
      ) const;
      //- Detect if a point is 'inside' (closed) surfaces.
      //  Returns -1 if not, returns first surface it is.
      void findInside
      (
        const labelList& surfacesToTest,
        const pointField& pt,
        labelList& insideSurfaces
      ) const;
};
}  // namespace mousse
#endif
