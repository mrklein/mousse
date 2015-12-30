// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "refinement_distance_data.hpp"
// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::refinementDistanceData& wDist
)
{
  return os
    << wDist.level0Size_ << token::SPACE << wDist.origin_
    << token::SPACE << wDist.originLevel_;
}
mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::refinementDistanceData& wDist
)
{
  return is >> wDist.level0Size_ >> wDist.origin_ >> wDist.originLevel_;
}
