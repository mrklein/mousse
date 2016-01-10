// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SuperBeeLimiter
// Description
//   Class with limiter function which returns the limiter for the
//   SuperBee differencing scheme based on r obtained from the LimiterFunc
//   class.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   super_bee.cpp

#ifndef super_bee_hpp_
#define super_bee_hpp_

#include "vector.hpp"

namespace mousse
{
template<class LimiterFunc>
class SuperBeeLimiter
:
  public LimiterFunc
{
public:
  SuperBeeLimiter(Istream&)
  {}

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
    return max(max(min(2*r, 1), min(r, 2)), 0);
  }
};
}  // namespace mousse

#endif
