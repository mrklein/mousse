#ifndef CORE_GLOBAL_CONSTANTS_MATHEMATICAL_CONSTANTS_HPP_
#define CORE_GLOBAL_CONSTANTS_MATHEMATICAL_CONSTANTS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar.hpp"
namespace mousse
{
namespace constant
{
namespace mathematical
{
  static const char* const group = "mathematical";
  const scalar e(M_E);
  const scalar pi(M_PI);
  const scalar twoPi(2*pi);
  const scalar piByTwo(0.5*pi);
}  // namespace mathematical
}  // namespace constant
}  // namespace mousse
#endif
