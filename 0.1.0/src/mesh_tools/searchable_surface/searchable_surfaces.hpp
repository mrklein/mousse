#ifndef MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_SURFACES_HPP_
#define MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_SURFACES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableSurfaces
// Description
//   Container for searchableSurfaces.

#include "searchable_surface.hpp"
#include "label_pair.hpp"
#include "writer.hpp"
#include "ptr_list.hpp"


namespace mousse {

// Forward declaration of classes
class triSurface;


class searchableSurfaces
:
  public PtrList<searchableSurface>
{
  // Private data
    //- Surface names
    wordList names_;
    //- Region names per surface
    List<wordList> regionNames_;
    ////- From global region name to surface and region on surface
    //HashTable<labelPair> regionNames_;
    //- Indices of all surfaces. Precalculated and stored.
    labelList allSurfaces_;
  // Private Member Functions
    //- Is edge on face
    static bool connected
    (
      const triSurface& s,
      const label edgeI,
      const pointIndexHit& hit
    );
public:
  CLASS_NAME("searchableSurfaces");
  // Constructors
    //- Construct with length specified. Fill later.
    explicit searchableSurfaces(const label);
    ////- Construct from list of dictionaries
    //searchableSurfaces(const IOobject&, const PtrList<dictionary>&);
    //- Construct from dictionary and whether to construct names always
    //  as surfaceName "_" regionName (singleRegionName false) or
    //  for single region surfaces as surfaceName only (singleRegionName
    //  true)
    searchableSurfaces
    (
      const IOobject&,
      const dictionary&,
      const bool singleRegionName
    );
    //- Disallow default bitwise copy construct
    searchableSurfaces(const searchableSurfaces&) = delete;
    //- Disallow default bitwise assignment
    searchableSurfaces& operator=(const searchableSurfaces&) = delete;
  // Member Functions
    const wordList& names() const
    {
      return names_;
    }
    wordList& names()
    {
      return names_;
    }
    const List<wordList>& regionNames() const
    {
      return regionNames_;
    }
    List<wordList>& regionNames()
    {
      return regionNames_;
    }
    //- Find index of surface. Return -1 if not found.
    label findSurfaceID(const word& name) const;
    label findSurfaceRegionID
    (
      const word& surfaceName,
      const word& regionName
    ) const;
    // Multiple point queries.
      //- Find any intersection. Return hit point information and
      //  surface number. If multiple surfaces hit the first surface
      //  is returned, not necessarily the nearest (to start).
      void findAnyIntersection
      (
        const pointField& start,
        const pointField& end,
        labelList& surfaces,
        List<pointIndexHit>&
      ) const;
      //- Find all intersections in order from start to end. Returns for
      //  every hit the surface and the hit info.
      void findAllIntersections
      (
        const pointField& start,
        const pointField& end,
        labelListList& surfaces,
        List<List<pointIndexHit> >&
      ) const;
      //Find intersections of edge nearest to both endpoints.
      void findNearestIntersection
      (
        const pointField& start,
        const pointField& end,
        labelList& surface1,
        List<pointIndexHit>& hit1,
        labelList& surface2,
        List<pointIndexHit>& hit2
      ) const;
      //- Find nearest. Return -1 (and a miss()) or surface and nearest
      //  point.
      void findNearest
      (
        const pointField&,
        const scalarField& nearestDistSqr,
        labelList& surfaces,
        List<pointIndexHit>&
      ) const;
      void findNearest
      (
        const pointField& samples,
        const scalarField& nearestDistSqr,
        const labelList& regionIndices,
        labelList& nearestSurfaces,
        List<pointIndexHit>& nearestInfo
      ) const;
      //- Calculate bounding box
      boundBox bounds() const;
    // Single point queries
      //- Calculate point which is on a set of surfaces.
      pointIndexHit facesIntersection
      (
        const scalar initialDistSqr,
        const scalar convergenceDistSqr,
        const point& start
      ) const;
    // Checking
      //- Are all surfaces closed and manifold
      bool checkClosed(const bool report) const;
      //- Are all (triangulated) surfaces consistent normal orientation
      bool checkNormalOrientation(const bool report) const;
      //- Are all bounding boxes of similar size
      bool checkSizes(const scalar maxRatio, const bool report) const;
      //- Do surfaces self-intersect or intersect others
      bool checkIntersection
      (
        const scalar tol,
        const autoPtr<writer<scalar> >&,
        const bool report
      ) const;
      //- Check triangle quality
      bool checkQuality
      (
        const scalar minQuality,
        const bool report
      ) const;
      //- All topological checks. Return number of failed checks
      label checkTopology(const bool report) const;
      //- All geometric checks. Return number of failed checks
      label checkGeometry
      (
        const scalar maxRatio,
        const scalar tolerance,
        const autoPtr<writer<scalar> >& setWriter,
        const scalar minQuality,
        const bool report
      ) const;
      //- Write some stats
      void writeStats(const List<wordList>&, Ostream&) const;
  // Member Operators
    //- Return const and non-const reference to searchableSurface by index.
    using PtrList<searchableSurface>::operator[];
    //- Return const reference to searchableSurface by name.
    const searchableSurface& operator[](const word&) const;
    //- Return reference to searchableSurface by name.
    searchableSurface& operator[](const word&);
};
}  // namespace mousse
#endif
