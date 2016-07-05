#ifndef CORE_ALGORITHMS_DYNAMIC_TREE_DATA_POINT_HPP_
#define CORE_ALGORITHMS_DYNAMIC_TREE_DATA_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dynamicTreeDataPoint
// Description
//   Holds (reference to) pointField. Encapsulation of data needed for
//   octree searches.
//   Used for searching for nearest point. No bounding boxes around points.
//   Only overlaps and calcNearest are implemented, rest makes little sense.
//   Optionally works on subset of points.
// SourceFiles
//   dynamic_tree_data_point.cpp

#include "point_field.hpp"
#include "tree_bound_box.hpp"
#include "line_point_ref.hpp"
#include "volume_type.hpp"


namespace mousse {

// Forward declaration of classes
template<class Type> class dynamicIndexedOctree;
class dynamicTreeDataPoint
{
  // Private data
    const DynamicList<point>& points_;
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("dynamicTreeDataPoint");
  // Constructors
    //- Construct from List. Holds reference!
    dynamicTreeDataPoint(const DynamicList<point>& points);
  // Member Functions
    // Access
      inline label size() const
      {
        return points_.size();
      }
      //- Get representative point cloud for all shapes inside
      //  (one point per shape)
      const DynamicList<point>& shapePoints() const;
    // Search
      //- Get type (inside,outside,mixed,unknown) of point w.r.t. surface.
      //  Only makes sense for closed surfaces.
      volumeType getVolumeType
      (
        const dynamicIndexedOctree<dynamicTreeDataPoint>&,
        const point&
      ) const;
      //- Does (bb of) shape at index overlap bb
      bool overlaps
      (
        const label index,
        const treeBoundBox& sampleBb
      ) const;
      //- Check if any point on shape is inside sphere.
      bool overlaps
      (
        const label index,
        const point& centre,
        const scalar radiusSqr
      ) const;
      //- Calculates nearest (to sample) point in shape.
      //  Returns actual point and distance (squared)
      void findNearest
      (
        const labelUList& indices,
        const point& sample,
        scalar& nearestDistSqr,
        label& nearestIndex,
        point& nearestPoint
      ) const;
      //- Calculates nearest (to line) point in shape.
      //  Returns point and distance (squared)
      void findNearest
      (
        const labelUList& indices,
        const linePointRef& ln,
        treeBoundBox& tightest,
        label& minIndex,
        point& linePoint,
        point& nearestPoint
      ) const;
      //- Calculate intersection of shape with ray. Sets result
      //  accordingly
      bool intersects
      (
        const label /*index*/,
        const point& /*start*/,
        const point& /*end*/,
        point& /*result*/
      ) const
      {
        NOT_IMPLEMENTED
        (
          "dynamicTreeDataPoint::intersects"
          "(const label, const point&, const point&, point&)"
        );
        return false;
      }
};
}  // namespace mousse
#endif
