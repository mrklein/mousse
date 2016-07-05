#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_FILTERED_LINEAR3_V_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_FILTERED_LINEAR3_V_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::filteredLinear3VLimiter
// Description
//   Class to generate weighting factors for the filteredLinear3V
//   differencing scheme.
//   The aim is to remove high-frequency modes with "staggering"
//   characteristics from vector fields by comparing the face gradient in
//   the direction of maximum gradient with both neighbouring cell gradients
//   and introduce small amounts of upwind in order to damp these modes.
//   Used in conjunction with the template class LimitedScheme.

#include "vector.hpp"


namespace mousse {

template<class LimiterFunc>
class filteredLinear3VLimiter
:
  public LimiterFunc
{
  // Private data
    // Scaling corefficient for the gradient ratio,
    // 0 = linear
    // 1 = fully limited
    scalar k_;
public:
  filteredLinear3VLimiter(Istream& is)
  :
    k_{readScalar(is)}
  {
    if (k_ < 0 || k_ > 1) {
      FATAL_IO_ERROR_IN("filteredLinear3VLimiter(Istream& is)", is)
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
    vector dfV = phiN - phiP;
    // Scalar difference across the face
    // in the direction in which the difference is largest
    scalar df = dfV & dfV;
    // Twice differences across face-neighbour cells
    // in the direction in which the face-difference is largest
    scalar dP = 2*(dfV & (d & gradcP));
    scalar dN = 2*(dfV & (d & gradcN));
    // Calculate the limiter
    scalar limiter = 1 - k_*(dN - df)*(dP - df)/max(sqr(dN + dP), SMALL);
    // Limit the limiter between linear and upwind
    return max(min(limiter, 1), 0);
  }
};
}  // namespace mousse
#endif
