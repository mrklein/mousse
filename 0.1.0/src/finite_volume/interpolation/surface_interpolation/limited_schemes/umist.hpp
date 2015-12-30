// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UMISTLimiter
// Description
//   Class with limiter function which returns the limiter for the
//   UMIST differencing scheme.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   umist.cpp
#ifndef umist_hpp_
#define umist_hpp_
#include "vector.hpp"
namespace mousse
{
template<class LimiterFunc>
class UMISTLimiter
:
  public LimiterFunc
{
public:
  UMISTLimiter(Istream&)
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
    scalar r = LimiterFunc::r
    (
      faceFlux, phiP, phiN, gradcP, gradcN, d
    );
    return max(min(min(min(2*r, 0.75*r + 0.25), 0.25*r + 0.75), 2), 0);
  }
};
}  // namespace mousse
#endif
