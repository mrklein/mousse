// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::treeDataFace
// Description
//   Encapsulation of data needed to search for faces.
// SourceFiles
//   tree_data_face.cpp
#ifndef tree_data_face_hpp_
#define tree_data_face_hpp_
#include "face.hpp"
#include "indexed_octree.hpp"
#include "tree_bound_box_list.hpp"
#include "packed_bool_list.hpp"
#include "primitive_mesh.hpp"
#include "volume_type.hpp"
namespace mousse
{
// Forward declaration of classes
//class primitiveMesh;
//template<class Type> class indexedOctree;
class polyPatch;
class treeDataFace
{
  // Static data
    //- Tolerance on linear dimensions
    static scalar tolSqr;
  // Private data
    const primitiveMesh& mesh_;
    //- Subset of faces to work on
    const labelList faceLabels_;
    //- Inverse of faceLabels. For every mesh whether face is in faceLabels.
    PackedBoolList isTreeFace_;
    //- Whether to precalculate and store face bounding box
    const bool cacheBb_;
    //- Face bounding boxes (valid only if cacheBb_)
    treeBoundBoxList bbs_;
  // Private Member Functions
    //- Calculate face bounding box
    treeBoundBox calcBb(const label cellI) const;
    //- Initialise all member data
    void update();
public:
  class findNearestOp
  {
    const indexedOctree<treeDataFace>& tree_;
  public:
    findNearestOp(const indexedOctree<treeDataFace>& tree);
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
    const indexedOctree<treeDataFace>& tree_;
  public:
    findIntersectOp(const indexedOctree<treeDataFace>& tree);
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
  ClassName("treeDataFace");
  // Constructors
    //- Construct from mesh and subset of faces.
    treeDataFace
    (
      const bool cacheBb,
      const primitiveMesh&,
      const labelUList&
    );
    //- Construct from mesh and subset of faces, transferring contents
    treeDataFace
    (
      const bool cacheBb,
      const primitiveMesh&,
      const Xfer<labelList>&
    );
    //- Construct from mesh. Uses all faces in mesh.
    treeDataFace(const bool cacheBb, const primitiveMesh&);
    //- Construct from mesh. Uses all faces in patch.
    treeDataFace(const bool cacheBb, const polyPatch&);
  // Member Functions
    // Access
      inline const labelList& faceLabels() const
      {
        return faceLabels_;
      }
      inline const primitiveMesh& mesh() const
      {
        return mesh_;
      }
      inline label size() const
      {
        return faceLabels_.size();
      }
      //- Get representative point cloud for all shapes inside
      //  (one point per shape)
      pointField shapePoints() const;
    // Search
      //- Get type (inside,outside,mixed,unknown) of point w.r.t. surface.
      //  Only makes sense for closed surfaces.
      volumeType getVolumeType
      (
        const indexedOctree<treeDataFace>&,
        const point&
      ) const;
      //- Does (bb of) shape at index overlap bb
      bool overlaps
      (
        const label index,
        const treeBoundBox& sampleBb
      ) const;
};
}  // namespace mousse
#endif
