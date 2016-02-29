#ifndef CORE_MESHES_MATCH_POINTS_HPP_
#define CORE_MESHES_MATCH_POINTS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   match_points.cpp
#include "scalar_field.hpp"
#include "point_field.hpp"
namespace mousse
{
//- Determine correspondence between pointFields. Gets passed
//  local matching distance per point. Returns map from 0 to 1. Returns
//  true if found corresponding point in 1 for every point in 0; false
//  otherwise. Compares mag(pt - origin) and does proper comparison for points
//  with (almost) identical mag. pts0 can be subset of pts1.
bool matchPoints
(
  const UList<point>& pts0,
  const UList<point>& pts1,
  const UList<scalar>& matchDistance,
  const bool verbose,
  labelList& from0To1,
  const point& origin = point::zero
);
//- Supply pts0Dir and pts1Dir. They are directions associated with the points
//  e.g., a face normal associated with each face centre.
//  A match between similar points is only allowed if their directions are
//  equal and opposite
bool matchPoints
(
  const UList<point>& pts0,
  const UList<point>& pts1,
  const UList<point>& pts0Dir,
  const UList<point>& pts1Dir,
  const UList<scalar>& matchDistance,
  const bool verbose,
  labelList& from0To1,
  const point& origin = point::zero
);
}  // namespace mousse
#endif
