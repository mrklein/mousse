// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_face_orientation.hpp"


// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::patchFaceOrientation& wDist
)
{
  return os << wDist.flipStatus_;
}


mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::patchFaceOrientation& wDist
)
{
  return is >> wDist.flipStatus_;
}

