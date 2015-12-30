// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "external_point_edge_point.hpp"
// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::externalPointEdgePoint& wDist
)
{
  return os << wDist.origin() << wDist.distSqr();
}
mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::externalPointEdgePoint& wDist
)
{
  return is >> wDist.origin_ >> wDist.distSqr_;
}
