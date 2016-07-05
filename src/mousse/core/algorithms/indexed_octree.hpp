#ifndef CORE_ALGORITHMS_INDEXED_OCTREE_HPP_
#define CORE_ALGORITHMS_INDEXED_OCTREE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::indexedOctree
// Description
//   Non-pointer based hierarchical recursive searching
// SourceFiles
//   indexed_octree.cpp

#include "tree_bound_box.hpp"
#include "point_index_hit.hpp"
#include "fixed_list.hpp"
#include "ostream.hpp"
#include "hash_set.hpp"
#include "label_bits.hpp"
#include "packed_list.hpp"
#include "volume_type.hpp"


namespace mousse {

// Forward declaration of classes
template<class Type> class indexedOctree;
template<class Type> Ostream& operator<<(Ostream&, const indexedOctree<Type>&);
class Istream;

TEMPLATE_NAME(indexedOctree);

template<class Type>
class indexedOctree
:
  public indexedOctreeName
{
public:
  // Data types
    //- Tree node. Has up pointer and down pointers.
    class node
    {
    public:
      //- Bounding box of this node
      treeBoundBox bb_;
      //- Parent node (index into nodes_ of tree)
      label parent_;
      //- IDs of the 8 nodes on all sides of the mid point
      FixedList<labelBits, 8> subNodes_;
      friend Ostream& operator<< (Ostream& os, const node& n)
      {
        return os << n.bb_ << token::SPACE
          << n.parent_ << token::SPACE << n.subNodes_;
      }
      friend Istream& operator>> (Istream& is, node& n)
      {
        return is >> n.bb_ >> n.parent_ >> n.subNodes_;
      }
      friend bool operator==(const node& a, const node& b)
      {
        return
          a.bb_ == b.bb_
        && a.parent_ == b.parent_
        && a.subNodes_ == b.subNodes_;
      }
      friend bool operator!=(const node& a, const node& b)
      {
        return !(a == b);
      }
    };
private:
  // Static data
    //- Relative peturbation tolerance. Determines when point is
    //  considered to be close to face/edge of bb of node.
    //  The tolerance is relative to the bounding box of the smallest
    //  node.
    static scalar perturbTol_;
  // Private data
    //- Underlying shapes for geometric queries.
    const Type shapes_;
    //- List of all nodes
    List<node> nodes_;
    //- List of all contents (referenced by those nodes that are contents)
    labelListList contents_;
    //- Per node per octant whether is fully inside/outside/mixed.
    mutable PackedList<2> nodeTypes_;
  // Private Member Functions
    //- Helper: does bb intersect a sphere around sample? Or is any
    //  corner point of bb closer than nearestDistSqr to sample.
    //  (bb is implicitly provided as parent bb + octant)
    static bool overlaps
    (
      const treeBoundBox& parentBb,
      const direction octant,
      const scalar nearestDistSqr,
      const point& sample
    );
    // Construction
      //- Split list of indices into 8 bins
      //  according to where they are in relation to mid.
      void divide
      (
        const labelList& indices,
        const treeBoundBox& bb,
        labelListList& result
      ) const;
      //- Subdivide the contents node at position contentI.
      //  Appends to contents.
      node divide
      (
        const treeBoundBox& bb,
        DynamicList<labelList>& contents,
        const label contentI
      ) const;
      //- Split any contents node with more than minSize elements.
      void splitNodes
      (
        const label minSize,
        DynamicList<node>& nodes,
        DynamicList<labelList>& contents
      ) const;
      static label compactContents
      (
        DynamicList<node>& nodes,
        DynamicList<labelList>& contents,
        const label compactLevel,
        const label nodeI,
        const label level,
        List<labelList>& compactedContents,
        label& compactI
      );
      //- Determine inside/outside per node (mixed if cannot be
      //  determined). Only valid for closed shapes.
      volumeType calcVolumeType(const label nodeI) const;
      //- Search cached volume type.
      volumeType getVolumeType(const label nodeI, const point&) const;
    // Query
      //- Find nearest point to line.
      template<class FindNearestOp>
      void findNearest
      (
        const label nodeI,
        const linePointRef& ln,
        treeBoundBox& tightest,
        label& nearestShapeI,
        point& linePoint,
        point& nearestPoint,
        const FindNearestOp& fnOp
      ) const;
      //- Return bbox of octant
      treeBoundBox subBbox
      (
        const label parentNodeI,
        const direction octant
      ) const;
      //- Helper: take a point on/close to face of bb and push it
      //  inside or outside of bb.
      static point pushPoint
      (
        const treeBoundBox&,
        const point&,
        const bool pushInside
      );
      //- Helper: take a point on face of bb and push it
      //  inside or outside of bb.
      static point pushPoint
      (
        const treeBoundBox&,
        const direction,
        const point&,
        const bool pushInside
      );
      //- Helper: take point on face(s) of bb and push it away from
      //  edges of face.
      static point pushPointIntoFace
      (
        const treeBoundBox& bb,
        const vector& dir,          // end-start
        const point& pt
      );
      ////- Push point on multiple faces away from any corner/edge.
      //static void checkMultipleFaces
      //(
      //    const treeBoundBox& bb,
      //    const vector& dir,          // end-start
      //    pointIndexHit& faceHitInfo,
      //    direction& faceID
      //);
      //- Walk to parent of node+octant.
      bool walkToParent
      (
        const label nodeI,
        const direction octant,
        label& parentNodeI,
        label& parentOctant
      ) const;
      //- Walk tree to neighbouring node. Return false if edge of tree
      //  hit.
      bool walkToNeighbour
      (
        const point& facePoint,
        const direction faceID,         // direction to walk in
        label& nodeI,
        direction& octant
      ) const;
      //- Debug: return verbose the bounding box faces
      static word faceString(const direction faceID);
      //- Traverse a node. If intersects a triangle return first
      // intersection point.
      // findAny=true : return any intersection
      // findAny=false: return nearest (to start) intersection
      template<class FindIntersectOp>
      void traverseNode
      (
        const bool findAny,
        const point& treeStart,
        const vector& treeVec,
        const point& start,
        const point& end,
        const label nodeI,
        const direction octantI,
        pointIndexHit& hitInfo,
        direction& faceID,
        const FindIntersectOp& fiOp
      ) const;
      //- Find any or nearest intersection
      template<class FindIntersectOp>
      pointIndexHit findLine
      (
        const bool findAny,
        const point& treeStart,
        const point& treeEnd,
        const label startNodeI,
        const direction startOctantI,
        const FindIntersectOp& fiOp,
        const bool verbose = false
      ) const;
      //- Find any or nearest intersection of line between start and end.
      template<class FindIntersectOp>
      pointIndexHit findLine
      (
        const bool findAny,
        const point& start,
        const point& end,
        const FindIntersectOp& fiOp
      ) const;
      //- Find all elements intersecting box.
      void findBox
      (
        const label nodeI,
        const treeBoundBox& searchBox,
        labelHashSet& elements
      ) const;
      //- Find all elements intersecting sphere.
      void findSphere
      (
        const label nodeI,
        const point& centre,
        const scalar radiusSqr,
        labelHashSet& elements
      ) const;
      template<class CompareOp>
      static void findNear
      (
        const scalar nearDist,
        const bool okOrder,
        const indexedOctree<Type>& tree1,
        const labelBits index1,
        const treeBoundBox& bb1,
        const indexedOctree<Type>& tree2,
        const labelBits index2,
        const treeBoundBox& bb2,
        CompareOp& cop
      );
    // Other
      //- Count number of elements on this and sublevels
      label countElements(const labelBits index) const;
      //- Dump node+octant to an obj file
      void writeOBJ(const label nodeI, const direction octant) const;
      //- From index into contents_ to subNodes_ entry
      static labelBits contentPlusOctant
      (
        const label i,
        const direction octant
      )
      {
        return labelBits(-i - 1, octant);
      }
      //- From index into nodes_ to subNodes_ entry
      static labelBits nodePlusOctant
      (
        const label i,
        const direction octant
      )
      {
        return labelBits(i + 1, octant);
      }
      //- From empty to subNodes_ entry
      static labelBits emptyPlusOctant
      (
        const direction octant
      )
      {
        return labelBits(0, octant);
      }
public:
    //- Get the perturbation tolerance
    static scalar& perturbTol();
  // Constructors
    //- Construct null
    indexedOctree(const Type& shapes);
    //- Construct from components
    indexedOctree
    (
      const Type& shapes,
      const List<node>& nodes,
      const labelListList& contents
    );
    //- Construct from shapes
    indexedOctree
    (
      const Type& shapes,
      const treeBoundBox& bb,
      const label maxLevels,          // maximum number of levels
      const scalar maxLeafRatio,      // how many elements per leaf
      const scalar maxDuplicity       // in how many leaves is a shape on
                      // average
    );
    //- Construct from Istream
    indexedOctree(const Type& shapes, Istream& is);
    //- Clone
    autoPtr<indexedOctree<Type> > clone() const
    {
      return autoPtr<indexedOctree<Type> >
      (
        new indexedOctree<Type>(*this)
      );
    }
  // Member Functions
    // Access
      //- Reference to shape
      const Type& shapes() const
      {
        return shapes_;
      }
      //- List of all nodes
      const List<node>& nodes() const
      {
        return nodes_;
      }
      //- List of all contents (referenced by those nodes that are
      //  contents)
      const labelListList& contents() const
      {
        return contents_;
      }
      //- Top bounding box
      const treeBoundBox& bb() const
      {
        if (nodes_.empty())
        {
          FATAL_ERROR_IN("indexedOctree<Type>::bb() const")
            << "Tree is empty" << abort(FatalError);
        }
        return nodes_[0].bb_;
      }
    // Conversions for entries in subNodes_.
      static bool isContent(const labelBits i)
      {
        return i.val() < 0;
      }
      static bool isEmpty(const labelBits i)
      {
        return i.val() == 0;
      }
      static bool isNode(const labelBits i)
      {
        return i.val() > 0;
      }
      static label getContent(const labelBits i)
      {
        if (!isContent(i))
        {
          FATAL_ERROR_IN("getContent(const label)")
            << abort(FatalError);
        }
        return -i.val()-1;
      }
      static label getNode(const labelBits i)
      {
        if (!isNode(i))
        {
          FATAL_ERROR_IN("getNode(const label)")
            << abort(FatalError);
        }
        return i.val() - 1;
      }
      static direction getOctant(const labelBits i)
      {
        return i.bits();
      }
    // Queries
      pointIndexHit findNearest
      (
        const point& sample,
        const scalar nearestDistSqr
      ) const;
      //- Calculate nearest point on nearest shape.
      //  Returns
      //  - bool : any point found nearer than nearestDistSqr
      //  - label: index in shapes
      //  - point: actual nearest point found
      template<class FindNearestOp>
      pointIndexHit findNearest
      (
        const point& sample,
        const scalar nearestDistSqr,
        const FindNearestOp& fnOp
      ) const;
      //- Low level: calculate nearest starting from subnode.
      template<class FindNearestOp>
      void findNearest
      (
        const label nodeI,
        const point&,
        scalar& nearestDistSqr,
        label& nearestShapeI,
        point& nearestPoint,
        const FindNearestOp& fnOp
      ) const;
      //- Find nearest to line.
      //  Returns
      //  - bool : any point found?
      //  - label: index in shapes
      //  - point: actual nearest point found
      //  sets:
      //  - linePoint : corresponding nearest point on line
      pointIndexHit findNearest
      (
        const linePointRef& ln,
        treeBoundBox& tightest,
        point& linePoint
      ) const;
      template<class FindNearestOp>
      pointIndexHit findNearest
      (
        const linePointRef& ln,
        treeBoundBox& tightest,
        point& linePoint,
        const FindNearestOp& fnOp
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
      //- Find nearest intersection of line between start and end.
      template<class FindIntersectOp>
      pointIndexHit findLine
      (
        const point& start,
        const point& end,
        const FindIntersectOp& fiOp
      ) const;
      //- Find any intersection of line between start and end.
      template<class FindIntersectOp>
      pointIndexHit findLineAny
      (
        const point& start,
        const point& end,
        const FindIntersectOp& fiOp
      ) const;
      //- Find (in no particular order) indices of all shapes inside or
      //  overlapping bounding box (i.e. all shapes not outside box)
      labelList findBox(const treeBoundBox& bb) const;
      //- Find (in no particular order) indices of all shapes inside or
      //  overlapping a bounding sphere (i.e. all shapes not outside
      //  sphere)
      labelList findSphere
      (
        const point& centre,
        const scalar radiusSqr
      ) const;
      //- Find deepest node (as parent+octant) containing point. Starts
      //  off from starting index in nodes_ (use 0 to start from top)
      //  Use getNode and getOctant to extract info, or call findIndices.
      labelBits findNode(const label nodeI, const point&) const;
      //- Find shape containing point. Only implemented for certain
      //  shapes.
      label findInside(const point&) const;
      //- Find the shape indices that occupy the result of findNode
      const labelList& findIndices(const point&) const;
      //- Determine type (inside/outside/mixed) for point. unknown if
      //  cannot be determined (e.g. non-manifold surface)
      volumeType getVolumeType(const point&) const;
      //- Helper function to return the side. Returns outside if
      //  outsideNormal&vec >= 0, inside otherwise
      static volumeType getSide
      (
        const vector& outsideNormal,
        const vector& vec
      );
      //- Helper: does bb intersect a sphere around sample? Or is any
      //  corner point of bb closer than nearestDistSqr to sample.
      static bool overlaps
      (
        const point& bbMin,
        const point& bbMax,
        const scalar nearestDistSqr,
        const point& sample
      );
      //- Find near pairs and apply CompareOp to them.
      //  tree2 can be *this or different tree.
      template<class CompareOp>
      void findNear
      (
        const scalar nearDist,
        const indexedOctree<Type>& tree2,
        CompareOp& cop
      ) const;
    // Write
      //- Print tree. Either print all indices (printContent = true) or
      //  just size of contents nodes.
      void print
      (
        prefixOSstream&,
        const bool printContents,
        const label
      ) const;
      bool write(Ostream& os) const;
  // IOstream Operators
    friend Ostream& operator<< <Type>(Ostream&, const indexedOctree<Type>&);
};

}  // namespace mousse

#include "indexed_octree.ipp"

#endif