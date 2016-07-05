#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_FILTERED_LINEAR2_V_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_FILTERED_LINEAR2_V_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::filteredLinear2VLimiter
// Description
//   Class to generate weighting factors for the filteredLinear2V
//   differencing scheme.
//   The aim is to remove high-frequency modes with "staggering"
//   characteristics from vector fields by comparing the face gradient in
//   the direction of maximum gradient with both neighbouring cell gradients
//   and introduce small amounts of upwind in order to damp these modes.
//   Used in conjunction with the template class LimitedScheme.

#include "vector.hpp"


namespace mousse {

template<class LimiterFunc>
class filteredLinear2VLimiter
:
  public LimiterFunc
{
  // Private data
    // Scaling corefficient for the gradient ratio,
    // 0 = linear
    // 1 = fully limited
    scalar k_;
    // Maximum allowed overshoot/undershoot relative to the difference
    // across the face.
    // On input:
    //     0 = no overshoot/undershoot
    //     1 = overshoot/undershoot equal to the difference across the face
    // Note: After input 1 is added to l_
    scalar l_;
public:
  filteredLinear2VLimiter(Istream& is)
  :
    k_{readScalar(is)},
    l_{readScalar(is)}
  {
    if (k_ < 0 || k_ > 1) {
      FATAL_IO_ERROR_IN("filteredLinear2VLimiter(Istream& is)", is)
        << "coefficient = " << k_
        << " should be >= 0 and <= 1"
        << exit(FatalIOError);
    }
    if (l_ < 0 || l_ > 1) {
      FATAL_IO_ERROR_IN("filteredLinear2VLimiter(Istream& is)", is)
        << "coefficient = " << l_
        << " should be >= 0 and <= 1"
        << exit(FatalIOError);
    }
    l_ += 1.0;
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
    scalar tdcP = 2*(dfV & (d & gradcP));
    scalar tdcN = 2*(dfV & (d & gradcN));
    // Calculate the limiter according to the sign of the face difference
    scalar limiter;
    if (df > 0) {
      limiter = l_ - k_*min(max(df - tdcP, 0), max(df - tdcN, 0))
        /(max(mag(df), max(mag(tdcP), mag(tdcN))) + VSMALL);
    } else {
      limiter = l_ - k_*min(max(tdcP - df, 0), max(tdcN - df, 0))
        /(max(mag(df), max(mag(tdcP), mag(tdcN))) + VSMALL);
    }
    // Limit the limiter between linear and upwind
    return max(min(limiter, 1), 0);
  }
};
}  // namespace mousse
#endif
