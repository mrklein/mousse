#ifndef MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_PLANE_HPP_
#define MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_PLANE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchablePlane
// Description
//   Searching on (infinite) plane. See plane.H
// SourceFiles
//   searchable_plane.cpp
#include "searchable_surface.hpp"
#include "plane.hpp"
namespace mousse
{
// Forward declaration of classes
class searchablePlane
:
  public searchableSurface,
  public plane
{
private:
  // Private Member Data
    mutable wordList regions_;
  // Private Member Functions
    pointIndexHit findLine
    (
      const point& start,
      const point& end
    ) const;
    //- Return the boundBox of the plane
    boundBox calcBounds() const;
public:
  //- Runtime type information
  TYPE_NAME("searchablePlane");
  // Constructors
    //- Construct from components
    searchablePlane
    (
      const IOobject& io,
      const point& basePoint,
      const vector& normal
    );
    //- Construct from dictionary (used by searchableSurface)
    searchablePlane
    (
      const IOobject& io,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    searchablePlane(const searchablePlane&) = delete;
    //- Disallow default bitwise assignment
    searchablePlane& operator=(const searchablePlane&) = delete;
  //- Destructor
  virtual ~searchablePlane();
  // Member Functions
    virtual const wordList& regions() const;
    //- Whether supports volume type below
    virtual bool hasVolumeType() const
    {
      return false;
    }
    //- Range of local indices that can be returned.
    virtual label size() const
    {
      return 1;
    }
    //- Get representative set of element coordinates
    //  Usually the element centres (should be of length size()).
    virtual tmp<pointField> coordinates() const
    {
      tmp<pointField> tCtrs(new pointField(1, refPoint()));
      return tCtrs;
    }
    //- Get bounding spheres (centre and radius squared), one per element.
    //  Any point on element is guaranteed to be inside.
    //  Note: radius limited to sqr(GREAT)
    virtual void boundingSpheres
    (
      pointField& centres,
      scalarField& radiusSqr
    ) const;
    //- Get the points that define the surface.
    virtual tmp<pointField> points() const
    {
      return coordinates();
    }
    //- Does any part of the surface overlap the supplied bound box?
    virtual bool overlaps(const boundBox&) const
    {
      NOT_IMPLEMENTED
      (
        "searchablePlane::overlaps(const boundBox&) const"
      );
      return false;
    }
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
        NOT_IMPLEMENTED("searchablePlane::writeData(Ostream&) const");
        return false;
      }
};
}  // namespace mousse
#endif
