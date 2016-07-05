// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face.hpp"


mousse::scalar mousse::face::contactSphereDiameter
(
  const point& p,
  const vector& n,
  const pointField& meshPoints
) const
{
  scalar magN = mousse::mag(n);
  vector n1 = n/(magN + SMALL);
  vector n2 = normal(meshPoints);
  n2 /= mousse::mag(n2) + SMALL;
  return 2*((centre(meshPoints) - p) & n2)/((n1 & n2) - 1.0);
}
