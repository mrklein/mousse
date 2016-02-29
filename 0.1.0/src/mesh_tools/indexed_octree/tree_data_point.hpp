#ifndef MESH_TOOLS_INDEXED_OCTREE_TREE_DATA_POINT_HPP_
#define MESH_TOOLS_INDEXED_OCTREE_TREE_DATA_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::treeDataPoint
// Description
//   Holds (reference to) pointField. Encapsulation of data needed for
//   octree searches.
//   Used for searching for nearest point. No bounding boxes around points.
//   Only overlaps and calcNearest are implemented, rest makes little sense.
//   Optionally works on subset of points.
// SourceFiles
//   tree_data_point.cpp
#include "point_field.hpp"
#include "tree_bound_box.hpp"
#include "line_point_ref.hpp"
#include "volume_type.hpp"
namespace mousse
{
// Forward declaration of classes
template<class Type> class indexedOctree;
class treeDataPoint
{
  // Private data
    const pointField& points_;
    //- Subset of points to work on (or empty)
    const labelList pointLabels_;
    const bool useSubset_;
public:
  class findNearestOp
  {
    const indexedOctree<treeDataPoint>& tree_;
  public:
    findNearestOp(const indexedOctree<treeDataPoint>& tree);
    void operator()
    (
      const labelUList& indices,
      const point& sample,
      scalar& nearestDistSqr,
      label& minIndex,
      point& nearestPoint
    ) const;
    void operator()
    (
      const labelUList& indices,
      const linePointRef& ln,
      treeBoundBox& tightest,
      label& minIndex,
      point& linePoint,
      point& nearestPoint
    ) const;
  };
  class findIntersectOp
  {
  public:
    findIntersectOp(const indexedOctree<treeDataPoint>& tree);
    //- Calculate intersection of triangle with ray. Sets result
    //  accordingly
    bool operator()
    (
      const label index,
      const point& start,
      const point& end,
      point& intersectionPoint
    ) const;
  };
  // Declare name of the class and its debug switch
  CLASS_NAME("treeDataPoint");
  // Constructors
    //- Construct from pointField. Holds reference!
    treeDataPoint(const pointField&);
    //- Construct from subset of pointField. Holds reference!
    treeDataPoint(const pointField&, const labelList&);
  // Member Functions
    // Access
      inline label size() const
      {
        return
        (
          useSubset_
         ? pointLabels_.size()
         : points_.size()
        );
      }
      inline const labelList& pointLabels() const
      {
        return pointLabels_;
      }
      const pointField& points() const
      {
        return points_;
      }
      bool useSubset() const
      {
        return useSubset_;
      }
      //- Get representative point cloud for all shapes inside
      //  (one point per shape)
      pointField shapePoints() const;
    // Search
      //- Get type (inside,outside,mixed,unknown) of point w.r.t. surface.
      //  Only makes sense for closed surfaces.
      volumeType getVolumeType
      (
        const indexedOctree<treeDataPoint>&,
        const point&
      ) const;
      //- Does (bb of) shape at index overlap bb
      bool overlaps
      (
        const label index,
        const treeBoundBox& sampleBb
      ) const;
      //- Does shape at index overlap the sphere
      bool overlaps
      (
        const label index,
        const point& centre,
        const scalar radiusSqr
      ) const;
};
}  // namespace mousse
#endif
