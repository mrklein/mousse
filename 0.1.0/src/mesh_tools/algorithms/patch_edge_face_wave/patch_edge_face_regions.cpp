// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_edge_face_regions.hpp"
// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::patchEdgeFaceRegions& wDist
)
{
  return os << wDist.regions_;
}
mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::patchEdgeFaceRegions& wDist
)
{
  return is >> wDist.regions_;
}
