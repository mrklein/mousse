// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MUSCLLimiter
// Description
//   Class with limiter function which returns the limiter for the
//   van Leer's MUSCL differencing scheme.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   muscl.cpp

#ifndef muscl_hpp_
#define muscl_hpp_

#include "vector.hpp"

namespace mousse
{
template<class LimiterFunc>
class MUSCLLimiter
:
  public LimiterFunc
{
public:
  MUSCLLimiter(Istream&)
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
    return max(min(min(2*r, 0.5*r + 0.5), 2), 0);
  }
};
}  // namespace mousse
#endif
