#ifndef MESH_TOOLS_TRI_SURFACE_TRI_SURFACE_SEARCH_TRI_SURFACE_SEARCH_HPP_
#define MESH_TOOLS_TRI_SURFACE_TRI_SURFACE_SEARCH_TRI_SURFACE_SEARCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::triSurfaceSearch
// Description
//   Helper class to search on triSurface.
// SourceFiles
//   tri_surface_search.cpp
#include "point_field.hpp"
#include "bool_list.hpp"
#include "point_index_hit.hpp"
#include "indexed_octree.hpp"
#include "tree_data_tri_surface.hpp"
namespace mousse
{
// Forward declaration of classes
class triSurface;
class triSurfaceSearch
{
  // Private data
    //- Reference to surface to work on
    const triSurface& surface_;
    //- Optional tolerance to use in searches
    scalar tolerance_;
    //- Optional max tree depth of octree
    label maxTreeDepth_;
    //- Octree for searches
    mutable autoPtr<indexedOctree<treeDataTriSurface> > treePtr_;
  // Private Member Functions
    //- Check whether the current hit on the surface which lies on lineVec
    //  is unique.
    //  point : return 1 unique hit
    //  edge  : return 1 hit if in the cone of the edge faces
    //        : return 2 hits if outside or on the cone.
    bool checkUniqueHit
    (
      const pointIndexHit& currHit,
      const DynamicList<pointIndexHit, 1, 1>& hits,
      const vector& lineVec
    ) const;
public:
  // Constructors
    //- Construct from surface. Holds reference to surface!
    explicit triSurfaceSearch(const triSurface&);
    //- Construct from surface and dictionary.
    triSurfaceSearch(const triSurface&, const dictionary& dict);
    //- Construct from components
    triSurfaceSearch
    (
      const triSurface& surface,
      const scalar tolerance,
      const label maxTreeDepth
    );
    //- Disallow default bitwise copy construct
    triSurfaceSearch(const triSurfaceSearch&) = delete;
    //- Disallow default bitwise assignment
    triSurfaceSearch& operator=(const triSurfaceSearch&) = delete;
  //- Destructor
  ~triSurfaceSearch();
    //- Clear storage
    void clearOut();
  // Member Functions
    //- Demand driven construction of the octree
    const indexedOctree<treeDataTriSurface>& tree() const;
    //- Return reference to the surface.
    const triSurface& surface() const
    {
      return surface_;
    }
    //- Return tolerance to use in searches
    scalar tolerance() const
    {
      return tolerance_;
    }
    //- Return max tree depth of octree
    label maxTreeDepth() const
    {
      return maxTreeDepth_;
    }
    //- Calculate for each searchPoint inside/outside status.
    boolList calcInside(const pointField& searchPoints) const;
    void findNearest
    (
      const pointField& samples,
      const scalarField& nearestDistSqr,
      List<pointIndexHit>& info
    ) const;
    //- Calculate nearest point on surface for single searchPoint. Returns
    //  in pointIndexHit:
    //  - hit()      : whether nearest point found within bounding box
    //  - hitPoint() : coordinate of nearest point
    //  - index()    : surface triangle label
    pointIndexHit nearest(const point&, const vector& span) const;
    void findLine
    (
      const pointField& start,
      const pointField& end,
      List<pointIndexHit>& info
    ) const;
    void findLineAny
    (
      const pointField& start,
      const pointField& end,
      List<pointIndexHit>& info
    ) const;
    //- Calculate all intersections from start to end
    void findLineAll
    (
      const pointField& start,
      const pointField& end,
      List<List<pointIndexHit> >& info
    ) const;
};
}  // namespace mousse
#endif
