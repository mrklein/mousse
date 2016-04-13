#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LimitedLimiter
// Description
//   mousse::LimitedLimiter

#include "vector.hpp"


namespace mousse {

template<class LimitedScheme>
class LimitedLimiter
:
  public LimitedScheme
{
  //- Lower and upper bound of the variable
  scalar lowerBound_, upperBound_;
  void checkParameters(Istream& is)
  {
    if (lowerBound_ > upperBound_) {
      FATAL_IO_ERROR_IN("checkParameters()", is)
        << "Invalid bounds.  Lower = " << lowerBound_
        << "  Upper = " << upperBound_
        << ".  Lower bound is higher than the upper bound."
        << exit(FatalIOError);
    }
  }

public:
  LimitedLimiter
  (
    const scalar lowerBound,
    const scalar upperBound,
    Istream& is
  )
  :
    LimitedScheme{is},
    lowerBound_{lowerBound},
    upperBound_{upperBound}
  {
    checkParameters(is);
  }

  LimitedLimiter(Istream& is)
  :
    LimitedScheme{is},
    lowerBound_{readScalar(is)},
    upperBound_{readScalar(is)}
  {
    checkParameters(is);
  }

  scalar limiter
  (
    const scalar cdWeight,
    const scalar faceFlux,
    const scalar phiP,
    const scalar phiN,
    const vector& gradcP,
    const vector& gradcN,
    const vector& d
  ) const
  {
    // If not between the lower and upper bounds use upwind
    if ((faceFlux > 0 && (phiP < lowerBound_ || phiN > upperBound_))
        || (faceFlux < 0 && (phiN < lowerBound_ || phiP > upperBound_))) {
      return 0;
    } else {
      return LimitedScheme::limiter
      (
        cdWeight,
        faceFlux,
        phiP,
        phiN,
        gradcP,
        gradcN,
        d
      );
    }
  }
};

}  // namespace mousse

#endif

