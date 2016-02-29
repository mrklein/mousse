#ifndef CORE_MESHES_MERGE_POINTS_HPP_
#define CORE_MESHES_MERGE_POINTS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   merge_points.cpp
#include "scalar.hpp"
#include "label_list.hpp"
namespace mousse
{
//- Sorts and merges points. All points closer than/equal mergeTol get merged.
//  Returns the number of unique points and a map from old to new.
template<class Type>
label mergePoints
(
  const UList<Type>& points,
  const scalar mergeTol,
  const bool verbose,
  labelList& pointMap,
  const Type& origin = Type::zero
);
//- Sorts and merges points. Determines new points. Returns true if anything
//  merged (though newPoints still sorted even if not merged).
template<class Type>
bool mergePoints
(
  const UList<Type>& points,
  const scalar mergeTol,
  const bool verbose,
  labelList& pointMap,
  List<Type>& newPoints,
  const Type& origin = Type::zero
);
}  // namespace mousse
#ifdef NoRepository
#   include "merge_points.cpp"
#endif
#endif
