#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_PHI_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_PHI_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PhiLimiter
// Description
//   Class with limiter function which returns the limiter for the
//   Phi differencing scheme.
//   Used in conjunction with the template class PhiScheme.
// SourceFiles
//   phi.cpp


#include "vector.hpp"

namespace mousse
{
class PhiLimiter
{
  scalar k_;
public:
  PhiLimiter(Istream& is)
  :
    k_{readScalar(is)}
  {
    if (k_ < 0 || k_ > 1)
    {
      FATAL_IO_ERROR_IN("PhiLimiter(Istream& is)", is)
        << "coefficient = " << k_
        << " should be >= 0 and <= 1"
        << exit(FatalIOError);
    }
  }
  scalar limiter
  (
    const scalar cdWeight,
    const scalar faceFlux,
    const vector& PhiP,
    const vector& PhiN,
    const vector& Sf,
    const scalar&
  ) const
  {
    scalar phiP = Sf&PhiP;
    scalar phiN = Sf&PhiN;
    scalar phiU;
    if (faceFlux > 0)
    {
      phiU = phiP;
    }
    else
    {
      phiU = phiN;
    }
    scalar phiCD = cdWeight*phiP + (1 - cdWeight)*phiN;
    // Calculate the effective limiter for the Phi interpolation
    //scalar PLimiter =
    //    (1.0 - k_) + k_*(faceFlux - phiU)/stabilise(phiCD - phiU, SMALL);
    scalar PLimiter =
      ((faceFlux - phiU)/stabilise(phiCD - phiU, SMALL) + k_);
    // Limit the limiter between upwind and central
    return max(min(PLimiter, 1), 0);
  }
};
}  // namespace mousse
#endif
