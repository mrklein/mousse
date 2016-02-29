#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_CUBIC_V_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_CUBIC_V_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::limitedCubicVLimiter
// Description
//   Class with limiter function which returns the limiter for the
//   limitedCubicV differencing scheme based on r obtained from the LimiterFunc
//   class.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   limited_cubic_v.cpp
#include "vector.hpp"
namespace mousse
{
template<class LimiterFunc>
class limitedCubicVLimiter
:
  public LimiterFunc
{
  scalar k_;
  scalar twoByk_;
public:
  limitedCubicVLimiter(Istream& is)
  :
    k_(readScalar(is))
  {
    if (k_ < 0 || k_ > 1)
    {
      FATAL_IO_ERROR_IN("limitedCubicVLimiter(Istream& is)", is)
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
    vector fV = cdWeight*phiP + (1.0 - cdWeight)*phiN;
    scalar fVphiP = fV & phiP;
    scalar fVphiN = fV & phiN;
    scalar fVphiU;
    if (faceFlux > 0)
    {
      fVphiU = fVphiP;
    }
    else
    {
      fVphiU = fVphiN;
    }
    // Calculate the face value using cubic interpolation
    scalar fVphif =
      cdWeight*(fVphiP - 0.25*(fV & (d & gradcN)))
     + (1 - cdWeight)*(fVphiN + 0.25*(fV & (d & gradcP)));
    scalar fVphiCD = cdWeight*fVphiP + (1 - cdWeight)*fVphiN;
    // Calculate the effective limiter for the cubic interpolation
    scalar cubicLimiter =
      (fVphif - fVphiU)/stabilise(fVphiCD - fVphiU, SMALL);
    // Limit the limiter to obey the TVD constraint
    return max(min(min(twor, cubicLimiter), 2), 0);
  }
};
}  // namespace mousse
#endif
