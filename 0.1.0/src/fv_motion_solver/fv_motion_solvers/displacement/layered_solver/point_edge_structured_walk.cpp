// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_edge_structured_walk.hpp"
// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::pointEdgeStructuredWalk& wDist
)
{
  return os
    << wDist.point0_ << wDist.previousPoint_
    << wDist.dist_ << wDist.data_;
}
mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::pointEdgeStructuredWalk& wDist
)
{
  return is
    >> wDist.point0_ >> wDist.previousPoint_
    >> wDist.dist_ >> wDist.data_;
}
