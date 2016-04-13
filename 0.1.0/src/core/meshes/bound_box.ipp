// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bound_box.hpp"
#include "fixed_list.hpp"
#include "pstream_reduce_ops.hpp"


// Constructors 
template<unsigned Size>
mousse::boundBox::boundBox
(
  const UList<point>& points,
  const FixedList<label, Size>& indices,
  const bool doReduce
)
:
  min_{point::zero},
  max_{point::zero}
{
  // a FixedList is never empty
  if (points.empty()) {
    if (doReduce && Pstream::parRun()) {
      // Use values that get overwritten by reduce minOp, maxOp below
      min_ = point{VGREAT, VGREAT, VGREAT};
      max_ = point{-VGREAT, -VGREAT, -VGREAT};
    }
  } else {
    min_ = points[indices[0]];
    max_ = points[indices[0]];
    for (unsigned i=1; i < Size; ++i) {
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
template<unsigned Size>
bool mousse::boundBox::contains
(
  const UList<point>& points,
  const FixedList<label, Size>& indices
) const
{
  if (points.empty()) {
    return false;
  }
  // a FixedList is never empty
  FOR_ALL(indices, i) {
    if (!contains(points[indices[i]])) {
      return false;
    }
  }
  return true;
}


template<unsigned Size>
bool mousse::boundBox::containsAny
(
  const UList<point>& points,
  const FixedList<label, Size>& indices
) const
{
  if (points.empty()) {
    return false;
  }
  // a FixedList is never empty
  FOR_ALL(indices, i) {
    if (contains(points[indices[i]])) {
      return true;
    }
  }
  return false;
}
