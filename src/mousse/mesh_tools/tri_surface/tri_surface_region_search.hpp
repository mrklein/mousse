#ifndef MESH_TOOLS_TRI_SURFACE_TRI_SURFACE_SEARCH_TRI_SURFACE_REGION_SEARCH_HPP_
#define MESH_TOOLS_TRI_SURFACE_TRI_SURFACE_SEARCH_TRI_SURFACE_REGION_SEARCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::triSurfaceRegionSearch
// Description
//   Helper class to search on triSurface. Creates an octree for each region of
//   the surface and only searches on the specified regions.

#include "point_field.hpp"
#include "point_index_hit.hpp"
#include "tri_surface_search.hpp"
#include "labelled_tri.hpp"
#include "indirect_list.hpp"
#include "ptr_list.hpp"
#include "indexed_octree.hpp"


namespace mousse {

class triSurfaceRegionSearch
:
  public triSurfaceSearch
{
  // Private typedefs
    typedef PrimitivePatch
    <
      labelledTri,
      IndirectList,
      const pointField&
    > indirectTriSurface;
    typedef treeDataPrimitivePatch<indirectTriSurface>
      treeDataIndirectTriSurface;
    typedef indexedOctree<treeDataIndirectTriSurface> treeType;
  // Private data
    //- Surface is split into patches by region
    mutable PtrList<indirectTriSurface> indirectRegionPatches_;
    //- Search tree for each region
    mutable PtrList<treeType> treeByRegion_;
public:
  // Constructors
    //- Construct from surface. Holds reference to surface!
    explicit triSurfaceRegionSearch(const triSurface&);
    //- Construct from surface and dictionary. Holds reference to surface!
    triSurfaceRegionSearch(const triSurface&, const dictionary& dict);
    //- Disallow default bitwise copy construct
    triSurfaceRegionSearch(const triSurfaceRegionSearch&) = delete;
    //- Disallow default bitwise assignment
    triSurfaceRegionSearch& operator=(const triSurfaceRegionSearch&) = delete;
  //- Destructor
  ~triSurfaceRegionSearch();
    //- Clear storage
    void clearOut();
  // Member Functions
    // Access
      //- Demand driven construction of octree for each region.
      //  @todo Currently creates a tree for each region; could optimise
      //        by only constructing trees when they are in regionIndices
      const PtrList<treeType>& treeByRegion() const;
    // Query
      //- Find the nearest point on the surface out of the regions
      //  supplied in the list regionIndices. Ignores regions that are
      //  not specified
      void findNearest
      (
        const pointField& samples,
        const scalarField& nearestDistSqr,
        const labelList& regionIndices,
        List<pointIndexHit>& info
      ) const;
};
}  // namespace mousse
#endif
