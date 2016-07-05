// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spherical_tensor.hpp"


namespace mousse {

// Static Data Members
template<>
const char* const sphericalTensor::typeName = "sphericalTensor";
template<>
const char* sphericalTensor::componentNames[] = {"ii"};
template<>
const sphericalTensor sphericalTensor::zero(0);
template<>
const sphericalTensor sphericalTensor::one(1);
template<>
const sphericalTensor sphericalTensor::max(VGREAT);
template<>
const sphericalTensor sphericalTensor::min(-VGREAT);
template<>
const sphericalTensor sphericalTensor::I(1);
template<>
const sphericalTensor sphericalTensor::oneThirdI(1.0/3.0);
template<>
const sphericalTensor sphericalTensor::twoThirdsI(2.0/3.0);

}  // namespace mousse
