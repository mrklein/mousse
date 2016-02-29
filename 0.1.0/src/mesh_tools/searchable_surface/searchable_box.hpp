#ifndef MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_BOX_HPP_
#define MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_BOX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableBox
// Description
//   Searching on bounding box
// SourceFiles
//   searchable_box.cpp
#include "searchable_surface.hpp"
#include "tree_bound_box.hpp"
namespace mousse
{
// Forward declaration of classes
class searchableBox
:
  public searchableSurface,
  public treeBoundBox
{
private:
  // Private Member Data
    mutable wordList regions_;
  // Private Member Functions
    //- Project onto component dir of planePt and update index() (=face)
    void projectOntoCoordPlane
    (
      const direction dir,
      const point& planePt,
      pointIndexHit& info
    ) const;
    //- Inherit findNearest from searchableSurface
    using searchableSurface::findNearest;
    //- Returns miss or hit with face (0..5)
    pointIndexHit findNearest
    (
      const point& bbMid,
      const point& sample,
      const scalar nearestDistSqr
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("searchableBox");
  // Constructors
    //- Construct from components
    searchableBox(const IOobject& io, const treeBoundBox& bb);
    //- Construct from dictionary (used by searchableSurface)
    searchableBox
    (
      const IOobject& io,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    searchableBox(const searchableBox&) = delete;
    //- Disallow default bitwise assignment
    searchableBox& operator=(const searchableBox&) = delete;
  //- Destructor
  virtual ~searchableBox();
  // Member Functions
    virtual const wordList& regions() const;
    //- Whether supports volume type below
    virtual bool hasVolumeType() const
    {
      return true;
    }
    //- Range of local indices that can be returned.
    virtual label size() const
    {
      return 6;
    }
    //- Get representative set of element coordinates
    //  Usually the element centres (should be of length size()).
    virtual tmp<pointField> coordinates() const;
    //- Get bounding spheres (centre and radius squared), one per element.
    //  Any point on element is guaranteed to be inside.
    virtual void boundingSpheres
    (
      pointField& centres,
      scalarField& radiusSqr
    ) const;
    //- Get the points that define the surface.
    virtual tmp<pointField> points() const;
    // Does any part of the surface overlap the supplied bound box?
    virtual bool overlaps(const boundBox& bb) const
    {
      return boundBox::overlaps(bb);
    }
    // Single point queries.
      //- Calculate nearest point on surface.
      //  Returns
      //  - bool : any point found nearer than nearestDistSqr
      //  - label: relevant index in surface (=face 0..5)
      //  - point: actual nearest point found
      pointIndexHit findNearest
      (
        const point& sample,
        const scalar nearestDistSqr
      ) const;
      //- Calculate nearest point on edge.
      //  Returns
      //  - bool : any point found nearer than nearestDistSqr
      //  - label: relevant index in surface(=?)
      //  - point: actual nearest point found
      pointIndexHit findNearestOnEdge
      (
        const point& sample,
        const scalar nearestDistSqr
      ) const;
      //- Find nearest to segment.
      //  Returns
      //  - bool : any point found?
      //  - label: relevant index in shapes (=face 0..5)
      //  - point: actual nearest point found
      //  sets:
      //  - tightest  : bounding box
      //  - linePoint : corresponding nearest point on line
      pointIndexHit findNearest
      (
        const linePointRef& ln,
        treeBoundBox& tightest,
        point& linePoint
      ) const;
      //- Find nearest intersection of line between start and end.
      pointIndexHit findLine
      (
        const point& start,
        const point& end
      ) const;
      //- Find any intersection of line between start and end.
      pointIndexHit findLineAny
      (
        const point& start,
        const point& end
      ) const;
    // Multiple point queries.
      virtual void findNearest
      (
        const pointField& sample,
        const scalarField& nearestDistSqr,
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
    // regIOobject implementation
      bool writeData(Ostream&) const
      {
        NOT_IMPLEMENTED("searchableBox::writeData(Ostream&) const");
        return false;
      }
};
}  // namespace mousse
#endif
