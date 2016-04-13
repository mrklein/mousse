// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_spherical_tensor.hpp"


namespace mousse {

// Static Data Members
template<>
const char* const labelSphericalTensor::typeName = "labelSphericalTensor";
template<>
const char* labelSphericalTensor::componentNames[] = {"ii"};
template<>
const labelSphericalTensor labelSphericalTensor::zero(0);
template<>
const labelSphericalTensor labelSphericalTensor::one(1);
template<>
const labelSphericalTensor labelSphericalTensor::I(1);

}  // namespace mousse
