// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tree_bound_box.hpp"
#include "fixed_list.hpp"

// Constructors 
template<unsigned Size>
mousse::treeBoundBox::treeBoundBox
(
  const UList<point>& points,
  const FixedList<label, Size>& indices
)
:
  boundBox{points, indices, false}
{
  // points may be empty, but a FixedList is never empty
  if (points.empty())
  {
    WARNING_IN
    (
      "treeBoundBox::treeBoundBox"
      "(const UList<point>&, const FixedList<label, Size>&)"
    )
    << "cannot find bounding box for zero-sized pointField, "
    << "returning zero" << endl;
    return;
  }
}
