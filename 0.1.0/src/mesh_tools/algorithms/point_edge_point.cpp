// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_edge_point.hpp"


// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::pointEdgePoint& wDist
)
{
  return os << wDist.origin() << wDist.distSqr();
}


mousse::Istream& mousse::operator>>(mousse::Istream& is, mousse::pointEdgePoint& wDist)
{
  return is >> wDist.origin_ >> wDist.distSqr_;
}

