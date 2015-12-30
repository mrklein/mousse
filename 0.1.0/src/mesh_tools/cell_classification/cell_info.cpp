// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_info.hpp"
// Friend Operators 
mousse::Ostream& mousse::operator<<(mousse::Ostream& os, const mousse::cellInfo& wDist)
{
  return os << wDist.type_;
}
mousse::Istream& mousse::operator>>(mousse::Istream& is, mousse::cellInfo& wDist)
{
  return is >> wDist.type_;
}
