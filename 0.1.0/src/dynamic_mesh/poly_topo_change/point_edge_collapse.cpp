// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_edge_collapse.hpp"


// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::pointEdgeCollapse& wDist
)
{
  return os << wDist.collapsePoint_ << wDist.collapseIndex_
    << wDist.collapsePriority_;
}


mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::pointEdgeCollapse& wDist
)
{
  return is >> wDist.collapsePoint_ >> wDist.collapseIndex_
    >> wDist.collapsePriority_;
}

