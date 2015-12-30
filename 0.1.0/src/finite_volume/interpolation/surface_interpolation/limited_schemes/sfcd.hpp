// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SFCDLimiter
// Description
//   Class with limiter function which returns the limiter for the
//   SFCD differencing scheme based on phict obtained from the LimiterFunc
//   class.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   sfcd.cpp
#ifndef sfcd_hpp_
#define sfcd_hpp_
#include "vector.hpp"
namespace mousse
{
template<class LimiterFunc>
class SFCDLimiter
:
  public LimiterFunc
{
public:
  SFCDLimiter(Istream&)
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
    scalar phict = LimiterFunc::phict
    (
      faceFlux, phiP, phiN, gradcP, gradcN, d
    );
    scalar limitPhict = min(max(phict, 0), 0.5);
    return limitPhict/(1 - limitPhict);
  }
};
}  // namespace mousse
#endif
