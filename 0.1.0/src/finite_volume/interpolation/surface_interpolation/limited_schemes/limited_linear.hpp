#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_LINEAR_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_LINEAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::limitedLinearLimiter
// Description
//   Class with limiter function which returns the limiter for the
//   TVD limited linear differencing scheme based on r obtained from the
//   LimiterFunc class.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   limited_linear.cpp
#include "vector.hpp"
namespace mousse
{
template<class LimiterFunc>
class limitedLinearLimiter
:
  public LimiterFunc
{
  scalar k_;
  scalar twoByk_;
public:
  limitedLinearLimiter(Istream& is)
  :
    k_{readScalar(is)}
  {
    if (k_ < 0 || k_ > 1)
    {
      FATAL_IO_ERROR_IN("limitedLinearLimiter(Istream& is)", is)
        << "coefficient = " << k_
        << " should be >= 0 and <= 1"
        << exit(FatalIOError);
    }
    // Avoid the /0 when k_ = 0
    twoByk_ = 2.0/max(k_, SMALL);
  }

  scalar limiter
  (
    const scalar /*cdWeight*/,
    const scalar faceFlux,
    const typename LimiterFunc::phiType& phiP,
    const typename LimiterFunc::phiType& phiN,
    const typename LimiterFunc::gradPhiType& gradcP,
    const typename LimiterFunc::gradPhiType& gradcN,
    const vector& d
  ) const
  {
    scalar r = LimiterFunc::r
    (
      faceFlux, phiP, phiN, gradcP, gradcN, d
    );
    return max(min(twoByk_*r, 1), 0);
  }
};
}  // namespace mousse
#endif
