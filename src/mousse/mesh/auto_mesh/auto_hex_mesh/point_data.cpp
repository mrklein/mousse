// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_data.hpp"


// Friend Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const pointData& wDist)
{
  if (os.format() == IOstream::ASCII) {
    return os
      << static_cast<const pointEdgePoint&>(wDist)
      << token::SPACE << wDist.s() << token::SPACE << wDist.v();
  } else {
    return os
      << static_cast<const pointEdgePoint&>(wDist)
      << wDist.s() << wDist.v();
  }
}


mousse::Istream& mousse::operator>>(Istream& is, pointData& wDist)
{
  return is >> static_cast<pointEdgePoint&>(wDist) >> wDist.s_ >> wDist.v_;
}

