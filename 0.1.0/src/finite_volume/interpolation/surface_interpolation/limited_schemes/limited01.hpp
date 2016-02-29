#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED01_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED01_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Limited01Limiter
// Description
//   A LimitedLimiter with the range 0-1
#include "limited.hpp"
namespace mousse
{
template<class LimitedScheme>
class Limited01Limiter
:
  public LimitedLimiter<LimitedScheme>
{
public:
  Limited01Limiter(Istream& is)
  :
    LimitedLimiter<LimitedScheme>(0, 1, is)
  {}
};
}  // namespace mousse
#endif
