// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tree_bound_box.hpp"
#include "list_ops.hpp"


// Static Data Members
const mousse::scalar mousse::treeBoundBox::great{GREAT};

const mousse::treeBoundBox mousse::treeBoundBox::greatBox
{
  {-GREAT, -GREAT, -GREAT},
  {GREAT, GREAT, GREAT}
};

const mousse::treeBoundBox mousse::treeBoundBox::invertedBox
{
  {GREAT, GREAT, GREAT},
  {-GREAT, -GREAT, -GREAT}
};

//! \cond - skip documentation : local scope only
const mousse::label facesArray[6][4] =
{
  {0, 4, 6, 2}, // left
  {1, 3, 7, 5}, // right
  {0, 1, 5, 4}, // bottom
  {2, 6, 7, 3}, // top
  {0, 2, 3, 1}, // back
  {4, 5, 7, 6}  // front
};
//! \endcond
const mousse::faceList mousse::treeBoundBox::faces
{
  initListList<face, label, 6, 4>(facesArray)
};
//! \cond - skip documentation : local scope only
const mousse::label edgesArray[12][2] =
{
  {0, 1}, // 0
  {1, 3},
  {2, 3}, // 2
  {0, 2},
  {4, 5}, // 4
  {5, 7},
  {6, 7}, // 6
  {4, 6},
  {0, 4}, // 8
  {1, 5},
  {3, 7}, // 10
  {2, 6}
};
//! \endcond
const mousse::edgeList mousse::treeBoundBox::edges
{
  //initListList<edge, label, 12, 2>(edgesArray)
  calcEdges(edgesArray)
};
const mousse::FixedList<mousse::vector, 6> mousse::treeBoundBox::faceNormals
{
  calcFaceNormals()
};


// Private Member Functions 
mousse::edgeList mousse::treeBoundBox::calcEdges(const label edgesArray[12][2])
{
  edgeList edges(12);
  FOR_ALL(edges, edgeI) {
    edges[edgeI][0] = edgesArray[edgeI][0];
    edges[edgeI][1] = edgesArray[edgeI][1];
  }
  return edges;
}


mousse::FixedList<mousse::vector, 6> mousse::treeBoundBox::calcFaceNormals()
{
  FixedList<vector, 6> normals;
  normals[LEFT] = {-1,  0,  0};
  normals[RIGHT] = {1,  0,  0};
  normals[BOTTOM] = {0, -1,  0};
  normals[TOP] = {0,  1,  0};
  normals[BACK] = {0,  0, -1};
  normals[FRONT] = {0,  0,  1};
  return normals;
}


// Constructors 
mousse::treeBoundBox::treeBoundBox(const UList<point>& points)
:
  boundBox{points, false}
{
  if (points.empty()) {
    WARNING_IN
    (
      "treeBoundBox::treeBoundBox(const UList<point>&)"
    )
    << "cannot find bounding box for zero-sized pointField, "
    << "returning zero" << endl;
    return;
  }
}


mousse::treeBoundBox::treeBoundBox
(
  const UList<point>& points,
  const labelUList& indices
)
:
  boundBox{points, indices, false}
{
  if (points.empty() || indices.empty()) {
    WARNING_IN
    (
      "treeBoundBox::treeBoundBox"
      "(const UList<point>&, const labelUList&)"
    )
    << "cannot find bounding box for zero-sized pointField, "
    << "returning zero" << endl;
    return;
  }
}


// Member Functions 
mousse::tmp<mousse::pointField> mousse::treeBoundBox::points() const
{
  tmp<pointField> tPts = tmp<pointField>{new pointField{8}};
  pointField& points = tPts();
  FOR_ALL(points, octant) {
    points[octant] = corner(octant);
  }
  return tPts;
}


mousse::treeBoundBox mousse::treeBoundBox::subBbox(const direction octant) const
{
  return subBbox(midpoint(), octant);
}


// Octant to bounding box using permutation only.
mousse::treeBoundBox mousse::treeBoundBox::subBbox
(
  const point& mid,
  const direction octant
) const
{
  if (octant > 7) {
    FATAL_ERROR_IN
    (
      "treeBoundBox::subBbox(const point&, const direction)"
    )
    << "octant should be [0..7]"
    << abort(FatalError);
  }
  // start with a copy of this bounding box and adjust limits accordingly
  treeBoundBox subBb{*this};
  point& bbMin = subBb.min();
  point& bbMax = subBb.max();
  if (octant & treeBoundBox::RIGHTHALF) {
    bbMin.x() = mid.x();    // mid -> max
  } else {
    bbMax.x() = mid.x();    // min -> mid
  }
  if (octant & treeBoundBox::TOPHALF) {
    bbMin.y() = mid.y();    // mid -> max
  } else {
    bbMax.y() = mid.y();    // min -> mid
  }
  if (octant & treeBoundBox::FRONTHALF) {
    bbMin.z() = mid.z();    // mid -> max
  } else {
    bbMax.z() = mid.z();    // min -> mid
  }
  return subBb;
}


// line intersection. Returns true if line (start to end) inside
// bb or intersects bb. Sets pt to intersection.
//
// Sutherlands algorithm:
//   loop
//     - start = intersection of line with one of the planes bounding
//       the bounding box
//     - stop if start inside bb (return true)
//     - stop if start and end in same 'half' (e.g. both above bb)
//       (return false)
//
// Uses posBits to efficiently determine 'half' in which start and end
// point are.
//
// Note:
//   - sets coordinate to exact position: e.g. pt.x() = min().x()
//     since plane intersect routine might have truncation error.
//     This makes sure that posBits tests 'inside'
bool mousse::treeBoundBox::intersects
(
  const point& overallStart,
  const vector& overallVec,
  const point& start,
  const point& end,
  point& pt,
  direction& ptOnFaces
) const
{
  const direction endBits = posBits(end);
  pt = start;
  // Allow maximum of 3 clips.
  for (label i = 0; i < 4; ++i) {
    direction ptBits = posBits(pt);
    if (ptBits == 0) {
      // pt inside bb
      ptOnFaces = faceBits(pt);
      return true;
    }
    if ((ptBits & endBits) != 0) {
      // pt and end in same block outside of bb
      ptOnFaces = faceBits(pt);
      return false;
    }
    if (ptBits & LEFTBIT) {
      // Intersect with plane V=min, n=-1,0,0
      if (mousse::mag(overallVec.x()) > VSMALL) {
        scalar s = (min().x() - overallStart.x())/overallVec.x();
        pt.x() = min().x();
        pt.y() = overallStart.y() + overallVec.y()*s;
        pt.z() = overallStart.z() + overallVec.z()*s;
      } else {
        // Vector not in x-direction. But still intersecting bb planes.
        // So must be close - just snap to bb.
        pt.x() = min().x();
      }
    } else if (ptBits & RIGHTBIT) {
      // Intersect with plane V=max, n=1,0,0
      if (mousse::mag(overallVec.x()) > VSMALL) {
        scalar s = (max().x() - overallStart.x())/overallVec.x();
        pt.x() = max().x();
        pt.y() = overallStart.y() + overallVec.y()*s;
        pt.z() = overallStart.z() + overallVec.z()*s;
      } else {
        pt.x() = max().x();
      }
    } else if (ptBits & BOTTOMBIT) {
      // Intersect with plane V=min, n=0,-1,0
      if (mousse::mag(overallVec.y()) > VSMALL) {
        scalar s = (min().y() - overallStart.y())/overallVec.y();
        pt.x() = overallStart.x() + overallVec.x()*s;
        pt.y() = min().y();
        pt.z() = overallStart.z() + overallVec.z()*s;
      } else {
        pt.x() = min().y();
      }
    } else if (ptBits & TOPBIT) {
      // Intersect with plane V=max, n=0,1,0
      if (mousse::mag(overallVec.y()) > VSMALL) {
        scalar s = (max().y() - overallStart.y())/overallVec.y();
        pt.x() = overallStart.x() + overallVec.x()*s;
        pt.y() = max().y();
        pt.z() = overallStart.z() + overallVec.z()*s;
      } else {
        pt.y() = max().y();
      }
    } else if (ptBits & BACKBIT) {
      // Intersect with plane V=min, n=0,0,-1
      if (mousse::mag(overallVec.z()) > VSMALL) {
        scalar s = (min().z() - overallStart.z())/overallVec.z();
        pt.x() = overallStart.x() + overallVec.x()*s;
        pt.y() = overallStart.y() + overallVec.y()*s;
        pt.z() = min().z();
      } else {
        pt.z() = min().z();
      }
    } else if (ptBits & FRONTBIT) {
      // Intersect with plane V=max, n=0,0,1
      if (mousse::mag(overallVec.z()) > VSMALL) {
        scalar s = (max().z() - overallStart.z())/overallVec.z();
        pt.x() = overallStart.x() + overallVec.x()*s;
        pt.y() = overallStart.y() + overallVec.y()*s;
        pt.z() = max().z();
      } else {
        pt.z() = max().z();
      }
    }
  }
  // Can end up here if the end point is on the edge of the boundBox
  return true;
}


bool mousse::treeBoundBox::intersects
(
  const point& start,
  const point& end,
  point& pt
) const
{
  direction ptBits;
  return intersects(start, end-start, start, end, pt, ptBits);
}


bool mousse::treeBoundBox::contains(const vector& dir, const point& pt) const
{
  //
  // Compare all components against min and max of bb
  //
  for (direction cmpt=0; cmpt<3; cmpt++) {
    if (pt[cmpt] < min()[cmpt]) {
      return false;
    } else if (pt[cmpt] == min()[cmpt]) {
      // On edge. Outside if direction points outwards.
      if (dir[cmpt] < 0)
      {
        return false;
      }
    }
    if (pt[cmpt] > max()[cmpt]) {
      return false;
    } else if (pt[cmpt] == max()[cmpt]) {
      // On edge. Outside if direction points outwards.
      if (dir[cmpt] > 0) {
        return false;
      }
    }
  }
  // All components inside bb
  return true;
}


// Code position of pt on bounding box faces
mousse::direction mousse::treeBoundBox::faceBits(const point& pt) const
{
  direction faceBits = 0;
  if (pt.x() == min().x()) {
    faceBits |= LEFTBIT;
  } else if (pt.x() == max().x()) {
    faceBits |= RIGHTBIT;
  }
  if (pt.y() == min().y()) {
    faceBits |= BOTTOMBIT;
  } else if (pt.y() == max().y()) {
    faceBits |= TOPBIT;
  }
  if (pt.z() == min().z()) {
    faceBits |= BACKBIT;
  } else if (pt.z() == max().z()) {
    faceBits |= FRONTBIT;
  }
  return faceBits;
}


// Code position of point relative to box
mousse::direction mousse::treeBoundBox::posBits(const point& pt) const
{
  direction posBits = 0;
  if (pt.x() < min().x()) {
    posBits |= LEFTBIT;
  } else if (pt.x() > max().x()) {
    posBits |= RIGHTBIT;
  }
  if (pt.y() < min().y()) {
    posBits |= BOTTOMBIT;
  } else if (pt.y() > max().y()) {
    posBits |= TOPBIT;
  }
  if (pt.z() < min().z()) {
    posBits |= BACKBIT;
  } else if (pt.z() > max().z()) {
    posBits |= FRONTBIT;
  }
  return posBits;
}


// nearest and furthest corner coordinate.
// !names of treeBoundBox::min() and treeBoundBox::max() are confusing!
void mousse::treeBoundBox::calcExtremities
(
  const point& pt,
  point& nearest,
  point& furthest
) const
{
  scalar nearX, nearY, nearZ;
  scalar farX, farY, farZ;
  if (mousse::mag(min().x() - pt.x()) < mousse::mag(max().x() - pt.x())) {
    nearX = min().x();
    farX = max().x();
  } else {
    nearX = max().x();
    farX = min().x();
  }
  if (mousse::mag(min().y() - pt.y()) < mousse::mag(max().y() - pt.y())) {
    nearY = min().y();
    farY = max().y();
  } else {
    nearY = max().y();
    farY = min().y();
  }
  if (mousse::mag(min().z() - pt.z()) < mousse::mag(max().z() - pt.z())) {
    nearZ = min().z();
    farZ = max().z();
  } else {
    nearZ = max().z();
    farZ = min().z();
  }
  nearest = point(nearX, nearY, nearZ);
  furthest = point(farX, farY, farZ);
}


mousse::scalar mousse::treeBoundBox::maxDist(const point& pt) const
{
  point near, far;
  calcExtremities(pt, near, far);
  return mousse::mag(far - pt);
}


// Distance comparator
// Compare all vertices of bounding box against all of other bounding
// box to see if all vertices of one are nearer
mousse::label mousse::treeBoundBox::distanceCmp
(
  const point& pt,
  const treeBoundBox& other
) const
{
  //
  // Distance point <-> nearest and furthest away vertex of this
  //
  point nearThis, farThis;
  // get nearest and furthest away vertex
  calcExtremities(pt, nearThis, farThis);
  const scalar minDistThis =
    sqr(nearThis.x() - pt.x()) +  sqr(nearThis.y() - pt.y())
    +  sqr(nearThis.z() - pt.z());
  const scalar maxDistThis =
    sqr(farThis.x() - pt.x()) +  sqr(farThis.y() - pt.y())
    +  sqr(farThis.z() - pt.z());
  //
  // Distance point <-> other
  //
  point nearOther, farOther;
  // get nearest and furthest away vertex
  other.calcExtremities(pt, nearOther, farOther);
  const scalar minDistOther =
    sqr(nearOther.x() - pt.x()) +  sqr(nearOther.y() - pt.y())
    +  sqr(nearOther.z() - pt.z());
  const scalar maxDistOther =
    sqr(farOther.x() - pt.x()) +  sqr(farOther.y() - pt.y())
    +  sqr(farOther.z() - pt.z());
  //
  // Categorize
  //
  if (maxDistThis < minDistOther) {
    // All vertices of this are nearer to point than any vertex of other
    return -1;
  } else if (minDistThis > maxDistOther) {
    // All vertices of this are further from point than any vertex of other
    return 1;
  } else {
    // Mixed bag
    return 0;
  }
}


// Friend Operators 
bool mousse::operator==(const treeBoundBox& a, const treeBoundBox& b)
{
  return operator==
  (
    static_cast<const boundBox&>(a),
    static_cast<const boundBox&>(b)
  );
}


bool mousse::operator!=(const treeBoundBox& a, const treeBoundBox& b)
{
  return !(a == b);
}


// Ostream Operator 
mousse::Ostream& mousse::operator<<(Ostream& os, const treeBoundBox& bb)
{
  return os << static_cast<const boundBox&>(bb);
}


mousse::Istream& mousse::operator>>(Istream& is, treeBoundBox& bb)
{
  return is >> static_cast<boundBox&>(bb);
}

