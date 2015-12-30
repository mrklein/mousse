// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_topo_distance_data.hpp"
// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::pointTopoDistanceData& wDist
)
{
  return os << wDist.data_ << token::SPACE << wDist.distance_;
}
mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::pointTopoDistanceData& wDist
)
{
  return is >> wDist.data_ >> wDist.distance_;
}
