#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_VAN_LEER_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_VAN_LEER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::vanLeerLimiter
// Description
//   Class with limiter function which returns the limiter for the
//   vanLeer differencing scheme based on r obtained from the LimiterFunc
//   class.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   van_leer.cpp
#include "vector.hpp"
namespace mousse
{
template<class LimiterFunc>
class vanLeerLimiter
:
  public LimiterFunc
{
public:
  vanLeerLimiter(Istream&)
  {}
  scalar limiter
  (
    const scalar,
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
    return (r + mag(r))/(1 + mag(r));
  }
};
}  // namespace mousse
#endif
