// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableSphere
// Description
//   Searching on sphere
// SourceFiles
//   searchable_sphere.cpp
#ifndef searchable_sphere_hpp_
#define searchable_sphere_hpp_
#include "tree_bound_box.hpp"
#include "searchable_surface.hpp"
namespace mousse
{
// Forward declaration of classes
class searchableSphere
:
  public searchableSurface
{
private:
  // Private Member Data
    //- Centre point
    const point centre_;
    //- Radius
    const scalar radius_;
    //- Names of regions
    mutable wordList regions_;
  // Private Member Functions
    //- Inherit findNearest from searchableSurface
    using searchableSurface::findNearest;
    //- Find nearest point on sphere.
    pointIndexHit findNearest
    (
      const point& sample,
      const scalar nearestDistSqr
    ) const;
    //- Find intersection with sphere
    void findLineAll
    (
      const point& start,
      const point& end,
      pointIndexHit& near,
      pointIndexHit& far
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("searchableSphere");
  // Constructors
    //- Construct from components
    searchableSphere(const IOobject& io, const point&, const scalar radius);
    //- Construct from dictionary (used by searchableSurface)
    searchableSphere
    (
      const IOobject& io,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    searchableSphere(const searchableSphere&) = delete;
    //- Disallow default bitwise assignment
    searchableSphere& operator=(const searchableSphere&) = delete;
  //- Destructor
  virtual ~searchableSphere();
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
    virtual tmp<pointField> coordinates() const
    {
      tmp<pointField> tCtrs(new pointField(1, centre_));
      return tCtrs;
    }
    //- Get bounding spheres (centre and radius squared), one per element.
    //  Any point on element is guaranteed to be inside.
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
    virtual bool overlaps(const boundBox& bb) const;
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
        NOT_IMPLEMENTED("searchableSphere::writeData(Ostream&) const");
        return false;
      }
};
}  // namespace mousse
#endif
