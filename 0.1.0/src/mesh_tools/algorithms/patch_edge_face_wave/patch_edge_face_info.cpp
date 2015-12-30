// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_edge_face_info.hpp"
// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::patchEdgeFaceInfo& wDist
)
{
  return os << wDist.origin() << wDist.distSqr();
}
mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::patchEdgeFaceInfo& wDist
)
{
  return is >> wDist.origin_ >> wDist.distSqr_;
}
