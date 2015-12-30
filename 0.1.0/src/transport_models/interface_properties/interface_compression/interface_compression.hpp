// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interfaceCompressionLimiter
// Description
//   Interface compression scheme currently based on the generic limited
//   scheme although it does not use the NVD/TVD functions.
// SourceFiles
//   interface_compression.cpp
#ifndef interface_compression_hpp_
#define interface_compression_hpp_
#include "vector.hpp"
namespace mousse
{
class interfaceCompressionLimiter
{
public:
  interfaceCompressionLimiter(Istream&)
  {}
  scalar limiter
  (
    const scalar cdWeight,
    const scalar faceFlux,
    const scalar phiP,
    const scalar phiN,
    const vector&,
    const scalar
  ) const
  {
    // Quadratic compression scheme
    //return min(max(4*min(phiP*(1 - phiP), phiN*(1 - phiN)), 0), 1);
    // Quartic compression scheme
    return
      min(max(
      1 - max(sqr(1 - 4*phiP*(1 - phiP)), sqr(1 - 4*phiN*(1 - phiN))),
      0), 1);
  }
};
}  // namespace mousse
#endif
