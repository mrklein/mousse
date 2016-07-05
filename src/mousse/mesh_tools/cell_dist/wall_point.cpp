// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_point.hpp"


// Friend Operators 
mousse::Ostream& mousse::operator<<(mousse::Ostream& os, const mousse::wallPoint& wDist)
{
  return os << wDist.origin() << token::SPACE << wDist.distSqr();
}


mousse::Istream& mousse::operator>>(mousse::Istream& is, mousse::wallPoint& wDist)
{
  return is >> wDist.origin() >> wDist.distSqr();
}

