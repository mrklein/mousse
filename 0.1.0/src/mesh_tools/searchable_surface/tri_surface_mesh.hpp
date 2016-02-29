#ifndef MESH_TOOLS_SEARCHABLE_SURFACE_TRI_SURFACE_MESH_HPP_
#define MESH_TOOLS_SEARCHABLE_SURFACE_TRI_SURFACE_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::triSurfaceMesh
// Description
//   IOoject and searching on triSurface
//   Note: when constructing from dictionary has optional parameters:
//     - scale     : scaling factor.
//     - tolerance : relative tolerance for doing intersections
//            (see triangle::intersection)
//     - minQuality: discard triangles with low quality when getting normal
// SourceFiles
//   tri_surface_mesh.cpp
#include "tree_bound_box.hpp"
#include "searchable_surface.hpp"
#include "object_registry.hpp"
#include "indexed_octree.hpp"
#include "tree_data_tri_surface.hpp"
#include "tree_data_primitive_patch.hpp"
#include "tree_data_edge.hpp"
#include "edge_map.hpp"
#include "tri_surface.hpp"
#include "tri_surface_region_search.hpp"
namespace mousse
{
class triSurfaceMesh
:
  public searchableSurface,
  public objectRegistry,      // so we can store fields
  public triSurface,
  public triSurfaceRegionSearch
{
private:
  // Private member data
    //- Optional min triangle quality. Triangles below this get
    //  ignored for normal calculation
    scalar minQuality_;
    //- Search tree for boundary edges.
    mutable autoPtr<indexedOctree<treeDataEdge> > edgeTree_;
    //- Names of regions
    mutable wordList regions_;
    //- Is surface closed
    mutable label surfaceClosed_;
  // Private Member Functions
    ////- Helper: find instance of files without header
    //static word findRawInstance
    //(
    //    const Time&,
    //    const fileName&,
    //    const word&
    //);
    //- Check file existence
    static const fileName& checkFile
    (
      const fileName& fName,
      const fileName& objectName
    );
    //- Helper function for isSurfaceClosed
    static bool addFaceToEdge
    (
      const edge&,
      EdgeMap<label>&
    );
    //- Check whether surface is closed without calculating any permanent
    //  addressing.
    bool isSurfaceClosed() const;
    //- Steps to next intersection. Adds smallVec and starts tracking
    //  from there.
    static void getNextIntersections
    (
      const indexedOctree<treeDataTriSurface>& octree,
      const point& start,
      const point& end,
      const vector& smallVec,
      DynamicList<pointIndexHit, 1, 1>& hits
    );
public:
  //- Runtime type information
  TYPE_NAME("triSurfaceMesh");
  // Constructors
    //- Construct from triSurface
    triSurfaceMesh(const IOobject&, const triSurface&);
    //- Construct read.
    triSurfaceMesh(const IOobject& io);
    //- Construct from IO and dictionary (used by searchableSurface).
    //  Dictionary may contain a 'scale' entry (eg, 0.001: mm -> m)
    triSurfaceMesh
    (
      const IOobject& io,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    triSurfaceMesh(const triSurfaceMesh&) = delete;
    //- Disallow default bitwise assignment
    triSurfaceMesh& operator=(const triSurfaceMesh&) = delete;
  //- Destructor
  virtual ~triSurfaceMesh();
    //- Clear storage
    void clearOut();
  // Member Functions
    //- Move points
    virtual void movePoints(const pointField&);
    //- Demand driven construction of octree for boundary edges
    const indexedOctree<treeDataEdge>& edgeTree() const;
    // searchableSurface implementation
      virtual const wordList& regions() const;
      //- Whether supports volume type below. I.e. whether is closed.
      virtual bool hasVolumeType() const;
      //- Range of local indices that can be returned.
      virtual label size() const
      {
        return triSurface::size();
      }
      //- Get representative set of element coordinates
      //  Usually the element centres (should be of length size()).
      virtual tmp<pointField> coordinates() const;
      //- Get bounding spheres (centre and radius squared). Any point
      //  on surface is guaranteed to be inside.
      virtual void boundingSpheres
      (
        pointField& centres,
        scalarField& radiusSqr
      ) const;
      //- Get the points that define the surface.
      virtual tmp<pointField> points() const;
      // Does any part of the surface overlap the supplied bound box?
      virtual bool overlaps(const boundBox& bb) const;
      virtual void findNearest
      (
        const pointField& sample,
        const scalarField& nearestDistSqr,
        List<pointIndexHit>&
      ) const;
      virtual void findNearest
      (
        const pointField& sample,
        const scalarField& nearestDistSqr,
        const labelList& regionIndices,
        List<pointIndexHit>&
      ) const;
      virtual void findLine
      (
        const pointField& start,
        const pointField& end,
        List<pointIndexHit>&
      ) const;
      virtual void findLineAny
      (
        const pointField& start,
        const pointField& end,
        List<pointIndexHit>&
      ) const;
      //- Get all intersections in order from start to end.
      virtual void findLineAll
      (
        const pointField& start,
        const pointField& end,
        List<List<pointIndexHit> >&
      ) const;
      //- From a set of points and indices get the region
      virtual void getRegion
      (
        const List<pointIndexHit>&,
        labelList& region
      ) const;
      //- From a set of points and indices get the normal
      virtual void getNormal
      (
        const List<pointIndexHit>&,
        vectorField& normal
      ) const;
      //- Determine type (inside/outside/mixed) for point. unknown if
      //  cannot be determined (e.g. non-manifold surface)
      virtual void getVolumeType
      (
        const pointField&,
        List<volumeType>&
      ) const;
    // Other
      //- WIP. Store element-wise field.
      virtual void setField(const labelList& values);
      //- WIP. From a set of hits (points and
      //  indices) get the specified field. Misses do not get set.
      virtual void getField(const List<pointIndexHit>&, labelList&) const;
    // regIOobject implementation
      bool writeData(Ostream&) const
      {
        NOT_IMPLEMENTED("triSurfaceMesh::writeData(Ostream&) const");
        return false;
      }
      //- Write using given format, version and compression
      virtual bool writeObject
      (
        IOstream::streamFormat fmt,
        IOstream::versionNumber ver,
        IOstream::compressionType cmp
      ) const;
};
}  // namespace mousse
#endif
