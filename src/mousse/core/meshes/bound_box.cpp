// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bound_box.hpp"
#include "pstream_reduce_ops.hpp"
#include "tmp.hpp"


// Static Data Members
const mousse::scalar mousse::boundBox::great{VGREAT};

const mousse::boundBox mousse::boundBox::greatBox
(
  {-VGREAT, -VGREAT, -VGREAT},
  {VGREAT, VGREAT, VGREAT}
);

const mousse::boundBox mousse::boundBox::invertedBox
(
  {VGREAT, VGREAT, VGREAT},
  {-VGREAT, -VGREAT, -VGREAT}
);


// Private Member Functions 
void mousse::boundBox::calculate(const UList<point>& points, const bool doReduce)
{
  if (points.empty()) {
    min_ = point::zero;
    max_ = point::zero;
    if (doReduce && Pstream::parRun()) {
      // Use values that get overwritten by reduce minOp, maxOp below
      min_ = {VGREAT, VGREAT, VGREAT};
      max_ = {-VGREAT, -VGREAT, -VGREAT};
    }
  } else {
    min_ = points[0];
    max_ = points[0];
    for (label i = 1; i < points.size(); i++) {
      min_ = ::mousse::min(min_, points[i]);
      max_ = ::mousse::max(max_, points[i]);
    }
  }
  // Reduce parallel information
  if (doReduce) {
    reduce(min_, minOp<point>());
    reduce(max_, maxOp<point>());
  }
}


// Constructors 
mousse::boundBox::boundBox(const UList<point>& points, const bool doReduce)
:
  min_{point::zero},
  max_{point::zero}
{
  calculate(points, doReduce);
}


mousse::boundBox::boundBox(const tmp<pointField>& points, const bool doReduce)
:
  min_{point::zero},
  max_{point::zero}
{
  calculate(points(), doReduce);
  points.clear();
}


mousse::boundBox::boundBox
(
  const UList<point>& points,
  const labelUList& indices,
  const bool doReduce
)
:
  min_{point::zero},
  max_{point::zero}
{
  if (points.empty() || indices.empty()) {
    if (doReduce && Pstream::parRun()) {
      // Use values that get overwritten by reduce minOp, maxOp below
      min_ = {VGREAT, VGREAT, VGREAT};
      max_ = {-VGREAT, -VGREAT, -VGREAT};
    }
  } else {
    min_ = points[indices[0]];
    max_ = points[indices[0]];
    for (label i=1; i < indices.size(); ++i) {
      min_ = ::mousse::min(min_, points[indices[i]]);
      max_ = ::mousse::max(max_, points[indices[i]]);
    }
  }
  // Reduce parallel information
  if (doReduce) {
    reduce(min_, minOp<point>());
    reduce(max_, maxOp<point>());
  }
}


// Member Functions 
mousse::tmp<mousse::pointField> mousse::boundBox::points() const
{
  tmp<pointField> tPts = tmp<pointField>{new pointField{8}};
  pointField& pt = tPts();
  pt[0] = min_;                              // min-x, min-y, min-z
  pt[1] = {max_.x(), min_.y(), min_.z()};    // max-x, min-y, min-z
  pt[2] = {max_.x(), max_.y(), min_.z()};    // max-x, max-y, min-z
  pt[3] = {min_.x(), max_.y(), min_.z()};    // min-x, max-y, min-z
  pt[4] = {min_.x(), min_.y(), max_.z()};    // min-x, min-y, max-z
  pt[5] = {max_.x(), min_.y(), max_.z()};    // max-x, min-y, max-z
  pt[6] = max_;                              // max-x, max-y, max-z
  pt[7] = {min_.x(), max_.y(), max_.z()};    // min-x, max-y, max-z
  return tPts;
}


mousse::faceList mousse::boundBox::faces()
{
  faceList faces{6};
  FOR_ALL(faces, fI) {
    faces[fI].setSize(4);
  }
  faces[0][0] = 0;
  faces[0][1] = 1;
  faces[0][2] = 2;
  faces[0][3] = 3;
  faces[1][0] = 2;
  faces[1][1] = 6;
  faces[1][2] = 7;
  faces[1][3] = 3;
  faces[2][0] = 0;
  faces[2][1] = 4;
  faces[2][2] = 5;
  faces[2][3] = 1;
  faces[3][0] = 4;
  faces[3][1] = 7;
  faces[3][2] = 6;
  faces[3][3] = 5;
  faces[4][0] = 3;
  faces[4][1] = 7;
  faces[4][2] = 4;
  faces[4][3] = 0;
  faces[5][0] = 1;
  faces[5][1] = 5;
  faces[5][2] = 6;
  faces[5][3] = 2;
  return faces;
}


void mousse::boundBox::inflate(const scalar s)
{
  vector ext = vector::one*s*mag();
  min_ -= ext;
  max_ += ext;
}


bool mousse::boundBox::contains(const UList<point>& points) const
{
  if (points.empty()) {
    return true;
  }
  FOR_ALL(points, i) {
    if (!contains(points[i])) {
      return false;
    }
  }
  return true;
}


bool mousse::boundBox::contains
(
  const UList<point>& points,
  const labelUList& indices
) const
{
  if (points.empty() || indices.empty()) {
    return true;
  }
  FOR_ALL(indices, i) {
    if (!contains(points[indices[i]])) {
      return false;
    }
  }
  return true;
}


bool mousse::boundBox::containsAny(const UList<point>& points) const
{
  if (points.empty()) {
    return true;
  }
  FOR_ALL(points, i) {
    if (contains(points[i])) {
      return true;
    }
  }
  return false;
}


bool mousse::boundBox::containsAny
(
  const UList<point>& points,
  const labelUList& indices
) const
{
  if (points.empty() || indices.empty()) {
    return true;
  }
  FOR_ALL(indices, i) {
    if (contains(points[indices[i]])) {
      return true;
    }
  }
  return false;
}


mousse::point mousse::boundBox::nearest(const point& pt) const
{
  // Clip the point to the range of the bounding box
  const scalar surfPtx = mousse::max(mousse::min(pt.x(), max_.x()), min_.x());
  const scalar surfPty = mousse::max(mousse::min(pt.y(), max_.y()), min_.y());
  const scalar surfPtz = mousse::max(mousse::min(pt.z(), max_.z()), min_.z());
  return {surfPtx, surfPty, surfPtz};
}


// Ostream Operator 
mousse::Ostream& mousse::operator<<(Ostream& os, const boundBox& bb)
{
  if (os.format() == IOstream::ASCII) {
    os << bb.min_ << token::SPACE << bb.max_;
  } else {
    os.write
    (
      reinterpret_cast<const char*>(&bb.min_),
      sizeof(boundBox)
    );
  }
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const boundBox&)");
  return os;
}


mousse::Istream& mousse::operator>>(Istream& is, boundBox& bb)
{
  if (is.format() == IOstream::ASCII) {
    is >> bb.min_ >> bb.max_;
  } else {
    is.read
    (
      reinterpret_cast<char*>(&bb.min_),
      sizeof(boundBox)
    );
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, boundBox&)");
  return is;
}

