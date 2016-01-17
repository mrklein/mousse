// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef mathematical_constants_hpp_
#define mathematical_constants_hpp_
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
