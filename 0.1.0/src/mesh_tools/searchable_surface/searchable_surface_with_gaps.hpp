#ifndef MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_SURFACE_WITH_GAPS_HPP_
#define MESH_TOOLS_SEARCHABLE_SURFACE_SEARCHABLE_SURFACE_WITH_GAPS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableSurfaceWithGaps
// Description
//   searchableSurface using multiple slightly shifted underlying surfaces
//   to make sure pierces don't go through gaps:
//   - shift test vector with two small vectors (of size gap_) perpendicular
//    to the original.
//    Test with + and - this vector. Only if both register a hit is it seen
//    as one.
//   - extend the test vector slightly (with SMALL) to account for numerical
//    inaccuracies.

#include "searchable_surface.hpp"
#include "uptr_list.hpp"
#include "pair.hpp"


namespace mousse {

// Forward declaration of classes
class searchableSurfaceWithGaps
:
  public searchableSurface
{
private:
  // Private Member Data
    //- Gap size in metre
    const scalar gap_;
    //- Underlying geometry (size 1)
    UPtrList<searchableSurface> subGeom_;
  // Private Member Functions
    Pair<vector> offsetVecs(const point&, const point&) const;
    void offsetVecs
    (
      const pointField& start,
      const pointField& end,
      pointField& offset0,
      pointField& offset1
    ) const;
    static label countMisses
    (
      const List<pointIndexHit>& info,
      labelList& missMap
    );
    static label countMisses
    (
      const List<pointIndexHit>& plusInfo,
      const List<pointIndexHit>& minInfo,
      labelList& missMap
    );
public:
  //- Runtime type information
  TYPE_NAME("searchableSurfaceWithGaps");
  // Constructors
    //- Construct from dictionary (used by searchableSurface)
    searchableSurfaceWithGaps
    (
      const IOobject& io,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    searchableSurfaceWithGaps(const searchableSurfaceWithGaps&) = delete;
    //- Disallow default bitwise assignment
    searchableSurfaceWithGaps& operator=
    (
      const searchableSurfaceWithGaps&
    ) = delete;
  //- Destructor
  virtual ~searchableSurfaceWithGaps();
  // Member Functions
    const searchableSurface& surface() const
    {
      return subGeom_[0];
    }
    virtual const wordList& regions() const
    {
      return surface().regions();
    }
    //- Whether supports volume type below
    virtual bool hasVolumeType() const
    {
      return surface().hasVolumeType();
    }
    //- Range of local indices that can be returned.
    virtual label size() const
    {
      return surface().size();
    }
    //- Get representative set of element coordinates
    //  Usually the element centres (should be of length size()).
    virtual tmp<pointField> coordinates() const
    {
      return surface().coordinates();
    }
    //- Get bounding spheres (centre and radius squared), one per element.
    //  Any point on element is guaranteed to be inside.
    virtual void boundingSpheres
    (
      pointField& centres,
      scalarField& radiusSqr
    ) const
    {
      surface().boundingSpheres(centres, radiusSqr);
    }
    //- Get the points that define the surface.
    virtual tmp<pointField> points() const
    {
      return surface().points();
    }
    //- Does any part of the surface overlap the supplied bound box?
    //  Note: use perturbed surface? Since uses boundbox of points and
    //        not actual intersection chosen to use unperturbed surface.
    virtual bool overlaps(const boundBox& bb) const
    {
      return surface().overlaps(bb);
    }
    // Multiple point queries.
      //- Find nearest on original surface. Note:does not use perturbation
      //  and hence might be inconsistent with intersections.
      virtual void findNearest
      (
        const pointField& sample,
        const scalarField& nearestDistSqr,
        List<pointIndexHit>& info
      ) const
      {
        surface().findNearest
        (
          sample,
          nearestDistSqr,
          info
        );
      }
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
        const List<pointIndexHit>& info,
        labelList& region
      ) const
      {
        surface().getRegion(info, region);
      }
      //- From a set of points and indices get the normal
      virtual void getNormal
      (
        const List<pointIndexHit>& info,
        vectorField& normal
      ) const
      {
        surface().getNormal(info, normal);
      }
      //- Determine type (inside/outside/mixed) for point. unknown if
      //  cannot be determined (e.g. non-manifold surface)
      virtual void getVolumeType
      (
        const pointField& samples,
        List<volumeType>& info
      ) const
      {
        surface().getVolumeType(samples, info);
      }
    // Other
      //- WIP. Store element-wise field.
      virtual void setField(const labelList& values)
      {
        subGeom_[0].setField(values);
      }
      //- WIP. From a set of hits (points and
      //  indices) get the specified field. Misses do not get set. Return
      //  empty field if not supported.
      virtual void getField
      (
        const List<pointIndexHit>& info,
        labelList& values
      ) const
      {
        surface().getField(info, values);
      }
    // regIOobject implementation
      bool writeData(Ostream& os) const
      {
        return surface().writeData(os);
      }
};
}  // namespace mousse
#endif
