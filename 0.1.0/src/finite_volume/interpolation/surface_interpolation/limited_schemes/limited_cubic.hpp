#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_CUBIC_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_CUBIC_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::limitedCubicLimiter
// Description
//   Class with limiter function which returns the limiter for the
//   TVD limited centred-cubic differencing scheme based on r obtained from
//   the LimiterFunc class.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   limited_cubic.cpp
#include "vector.hpp"
namespace mousse
{
template<class LimiterFunc>
class limitedCubicLimiter
:
  public LimiterFunc
{
  scalar k_;
  scalar twoByk_;
public:
  limitedCubicLimiter(Istream& is)
  :
    k_(readScalar(is))
  {
    if (k_ < 0 || k_ > 1)
    {
      FATAL_IO_ERROR_IN("limitedCubicLimiter(Istream& is)", is)
        << "coefficient = " << k_
        << " should be >= 0 and <= 1"
        << exit(FatalIOError);
    }
    // Avoid the /0 when k_ = 0
    twoByk_ = 2.0/max(k_, SMALL);
  }
  scalar limiter
  (
    const scalar cdWeight,
    const scalar faceFlux,
    const typename LimiterFunc::phiType& phiP,
    const typename LimiterFunc::phiType& phiN,
    const typename LimiterFunc::gradPhiType& gradcP,
    const typename LimiterFunc::gradPhiType& gradcN,
    const vector& d
  ) const
  {
    scalar twor = twoByk_*LimiterFunc::r
    (
      faceFlux, phiP, phiN, gradcP, gradcN, d
    );
    scalar phiU;
    if (faceFlux > 0)
    {
      phiU = phiP;
    }
    else
    {
      phiU = phiN;
    }
    // Calculate the face value using cubic interpolation
    scalar phif =
      cdWeight*(phiP - 0.25*(d & gradcN))
     + (1 - cdWeight)*(phiN + 0.25*(d & gradcP));
    scalar phiCD = cdWeight*phiP + (1 - cdWeight)*phiN;
    // Calculate the effective limiter for the cubic interpolation
    scalar cubicLimiter = (phif - phiU)/stabilise(phiCD - phiU, SMALL);
    /*
    if (twor < 0.05)
    {
      cubicLimiter = twor;
    }
    return max(min(cubicLimiter, 2), 0);
    */
    // Limit the limiter to obey the TVD constraint
    return max(min(min(twor, cubicLimiter), 2), 0);
  }
};
}  // namespace mousse
#endif
