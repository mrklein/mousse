// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::GammaLimiter
// Description
//   Class with limiter function which returns the limiter for the
//   Gamma differencing scheme based on phict obtained from the LimiterFunc
//   class.
//   Used in conjunction with the template class LimitedScheme.
// SourceFiles
//   gamma.cpp
#ifndef gamma_hpp_
#define gamma_hpp_
#include "vector.hpp"
namespace mousse
{
template<class LimiterFunc>
class GammaLimiter
:
  public LimiterFunc
{
  scalar k_;
public:
  GammaLimiter(Istream& is)
  :
    k_(readScalar(is))
  {
    if (k_ < 0 || k_ > 1)
    {
      FatalIOErrorIn("GammaLimiter(Istream& is)", is)
        << "coefficient = " << k_
        << " should be >= 0 and <= 1"
        << exit(FatalIOError);
    }
    // Rescale k_ to be >= 0 and <= 0.5 (TVD conformant)
    // and avoid the /0 when k_ = 0
    k_ = max(k_/2.0, SMALL);
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
    scalar phict = LimiterFunc::phict
    (
      faceFlux, phiP, phiN, gradcP, gradcN, d
    );
    return min(max(phict/k_, 0), 1);
  }
};
}  // namespace mousse
#endif
