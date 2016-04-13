#ifndef MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_CYLINDER_HPP_
#define MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_CYLINDER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableCylinder
// Description
//   Searching on cylinder

#include "tree_bound_box.hpp"
#include "searchable_surface.hpp"


namespace mousse {

// Forward declaration of classes
class searchableCylinder
:
  public searchableSurface
{
private:
  // Private Member Data
    //- 'left' point
    const point point1_;
    //- 'right' point
    const point point2_;
    //- Length of vector point2-point1
    const scalar magDir_;
    //- Normalised vector point2-point1
    const vector unitDir_;
    //- Radius squared
    const scalar radius_;
    //- Names of regions
    mutable wordList regions_;
  // Private Member Functions
    //- Inherit findNearest from searchableSurface
    using searchableSurface::findNearest;
    //- Find nearest point on cylinder.
    pointIndexHit findNearest
    (
      const point& sample,
      const scalar nearestDistSqr
    ) const;
    scalar radius2(const point& pt) const;
    //- Find intersection with cylinder
    void findLineAll
    (
      const point& start,
      const point& end,
      pointIndexHit& near,
      pointIndexHit& far
    ) const;
    //- Return the boundBox of the cylinder
    boundBox calcBounds() const;
public:
  //- Runtime type information
  TYPE_NAME("searchableCylinder");
  // Constructors
    //- Construct from components
    searchableCylinder
    (
      const IOobject& io,
      const point&,
      const point&,
      const scalar radius
    );
    //- Construct from dictionary (used by searchableSurface)
    searchableCylinder
    (
      const IOobject& io,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    searchableCylinder(const searchableCylinder&) = delete;
    //- Disallow default bitwise assignment
    searchableCylinder& operator=(const searchableCylinder&) = delete;
  //- Destructor
  virtual ~searchableCylinder();
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
      return 1;
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
    //- Does any part of the surface overlap the supplied bound box?
    virtual bool overlaps(const boundBox&) const
    {
      NOT_IMPLEMENTED
      (
        "searchableCylinder::overlaps(const boundBox&) const"
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
        NOT_IMPLEMENTED("searchableCylinder::writeData(Ostream&) const");
        return false;
      }
};
}  // namespace mousse
#endif
