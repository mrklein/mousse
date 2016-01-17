// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::OSPRELimiter
// Description
//   Class with limiter function which returns the limiter for the
//   OSPRE differencing scheme based on r obtained from the LimiterFunc
//   class.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   ospre.cpp

#ifndef ospre_hpp_
#define ospre_hpp_

#include "vector.hpp"

namespace mousse
{
template<class LimiterFunc>
class OSPRELimiter
:
  public LimiterFunc
{
public:
  OSPRELimiter(Istream&)
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
    scalar rrp1 = r*(r + 1);
    return 1.5*rrp1/(rrp1 + 1);
  }
};
}  // namespace mousse
#endif
