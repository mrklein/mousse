// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_tensor.hpp"
// Static Data Members
namespace mousse
{
  template<>
  const char* const mousse::labelTensor::typeName = "labelTensor";
  template<>
  const char* mousse::labelTensor::componentNames[] =
  {
    "xx", "xy", "xz",
    "yx", "yy", "yz",
    "zx", "zy", "zz"
  };
  template<>
  const mousse::labelTensor mousse::labelTensor::zero
  (
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
  );
  template<>
  const mousse::labelTensor mousse::labelTensor::one
  (
    1, 1, 1,
    1, 1, 1,
    1, 1, 1
  );
}
