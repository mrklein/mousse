// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_edge_face_region.hpp"


// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::patchEdgeFaceRegion& wDist
)
{
  return os << wDist.region_;
}


mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::patchEdgeFaceRegion& wDist
)
{
  return is >> wDist.region_;
}
