// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::filteredLinear3Limiter
// Description
//   Class to generate weighting factors for the filteredLinear
//   differencing scheme.
//   The aim is to remove high-frequency modes with "staggering"
//   characteristics by comparing the face gradient with both neighbouring
//   cell gradients and introduce small amounts of upwind in order to damp
//   these modes.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   filtered_linear3.cpp

#ifndef filtered_linear3_hpp_
#define filtered_linear3_hpp_

#include "vector.hpp"

namespace mousse
{
template<class LimiterFunc>
class filteredLinear3Limiter
:
  public LimiterFunc
{
  // Private data
    // Scaling corefficient for the gradient ratio,
    // 0 = linear
    // 1 = fully limited
    scalar k_;
public:
  filteredLinear3Limiter(Istream& is)
  :
    k_{readScalar(is)}
  {
    if (k_ < 0 || k_ > 1)
    {
      FATAL_IO_ERROR_IN("filteredLinear3Limiter(Istream& is)", is)
        << "coefficient = " << k_
        << " should be >= 0 and <= 1"
        << exit(FatalIOError);
    }
  }

  scalar limiter
  (
    const scalar /*cdWeight*/,
    const scalar /*faceFlux*/,
    const typename LimiterFunc::phiType& phiP,
    const typename LimiterFunc::phiType& phiN,
    const typename LimiterFunc::gradPhiType& gradcP,
    const typename LimiterFunc::gradPhiType& gradcN,
    const vector& d
  ) const
  {
    // Difference across face
    scalar df = phiN - phiP;
    // Twice the differences across face-neighbour cells
    scalar dP = 2*(d & gradcP);
    scalar dN = 2*(d & gradcN);
    // Calculate the limiter
    scalar limiter = 1 - k_*(dN - df)*(dP - df)/max(sqr(dN + dP), SMALL);
    // Limit the limiter between linear and upwind
    return max(min(limiter, 1), 0);
  }
};
}  // namespace mousse
#endif
