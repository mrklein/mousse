// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symm_tensor_2d.hpp"
namespace mousse
{
// Static Data Members
template<>
const char* const symmTensor2D::typeName = "symmTensor2D";
template<>
const char* symmTensor2D::componentNames[] =
{
  "xx", "xy",
     "yy"
};
template<>
const symmTensor2D symmTensor2D::zero
(
  0, 0,
   0
);
template<>
const symmTensor2D symmTensor2D::one
(
  1, 1,
   1
);
template<>
const symmTensor2D symmTensor2D::max
(
  VGREAT, VGREAT,
      VGREAT
);
template<>
const symmTensor2D symmTensor2D::min
(
  -VGREAT, -VGREAT,
      -VGREAT
);
template<>
const symmTensor2D symmTensor2D::I
(
  1, 0,
   1
);
}  // namespace mousse
