// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "diag_tensor.hpp"
namespace mousse
{
// Static Data Members
template<>
const char* const diagTensor::typeName = "diagTensor";
template<>
const char* diagTensor::componentNames[] = {"xx", "yy", "zz"};
template<>
const diagTensor diagTensor::zero(0, 0, 0);
template<>
const diagTensor diagTensor::one(1, 1, 1);
template<>
const diagTensor diagTensor::max(VGREAT, VGREAT, VGREAT);
template<>
const diagTensor diagTensor::min(-VGREAT, -VGREAT, -VGREAT);
}  // namespace mousse
