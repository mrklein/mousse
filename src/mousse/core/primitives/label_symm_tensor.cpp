// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_symm_tensor.hpp"


namespace mousse {

// Static Data Members
template<>
const char* const labelSymmTensor::typeName = "labelSymmTensor";

template<>
const char* labelSymmTensor::componentNames[] =
{
  "xx", "xy", "xz",
        "yy", "yz",
              "zz"
};

template<>
const labelSymmTensor labelSymmTensor::zero
(
  0, 0, 0,
     0, 0,
        0
);

template<>
const labelSymmTensor labelSymmTensor::one
(
  1, 1, 1,
     1, 1,
        1
);

}  // namespace mousse
