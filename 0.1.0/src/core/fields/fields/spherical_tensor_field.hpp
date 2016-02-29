#ifndef CORE_FIELDS_FIELDS_SPHERICAL_TENSOR_FIELD_HPP_
#define CORE_FIELDS_FIELDS_SPHERICAL_TENSOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   spherical_tensor_field.cpp
#include "scalar_field.hpp"
#include "spherical_tensor.hpp"
#define TEMPLATE
#include "field_functions_m.hpp"
namespace mousse
{
typedef Field<sphericalTensor> sphericalTensorField;
UNARY_FUNCTION(scalar, sphericalTensor, tr)
UNARY_FUNCTION(sphericalTensor, sphericalTensor, sph)
UNARY_FUNCTION(scalar, sphericalTensor, det)
UNARY_FUNCTION(sphericalTensor, sphericalTensor, inv)
BINARY_OPERATOR(sphericalTensor, scalar, sphericalTensor, /, divide)
BINARY_TYPE_OPERATOR(sphericalTensor, scalar, sphericalTensor, /, divide)
}  // namespace mousse
#include "undef_field_functions_m.hpp"
#endif
