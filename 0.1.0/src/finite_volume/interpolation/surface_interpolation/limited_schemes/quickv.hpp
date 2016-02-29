#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_QUICKV_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_QUICKV_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::QUICKVLimiter
// Description
//   Class with limiter function which returns the limiter for the
//   quadratic-upwind differencing scheme.
//   Note that the weighting factors are not bounded between upwind and
//   central-differencing, some downwind contribution is possible although
//   the interpolate is limited to be between the upwind and downwind cell
//   values.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   quickv.cpp
#include "vector.hpp"
namespace mousse
{
template<class LimiterFunc>
class QUICKVLimiter
:
  public LimiterFunc
{
public:
  QUICKVLimiter(Istream&)
  {}
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
    vector gradfV = phiN - phiP;
    scalar phiCD = gradfV & (cdWeight*phiP + (1 - cdWeight)*phiN);
    scalar phiU, phif;
    if (faceFlux > 0)
    {
      phiU = gradfV & phiP;
      phif = 0.5*(phiCD + phiU + (1 - cdWeight)*(gradfV & (d & gradcP)));
    }
    else
    {
      phiU = gradfV & phiN;
      phif = 0.5*(phiCD + phiU - cdWeight*(gradfV & (d & gradcN)));
    }
    // Calculate the effective limiter for the QUICK interpolation
    scalar QLimiter = (phif - phiU)/stabilise(phiCD - phiU, SMALL);
    // Limit the limiter between upwind and downwind
    return max(min(QLimiter, 2), 0);
  }
};
}  // namespace mousse
#endif
