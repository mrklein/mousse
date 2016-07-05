#ifndef CORE_FIELDS_FIELD_FIELDS_TENSOR_FIELD_FIELD_HPP_
#define CORE_FIELDS_FIELD_FIELDS_TENSOR_FIELD_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spherical_tensor_field_field.hpp"
#include "symm_tensor_field_field.hpp"
#include "tensor.hpp"
#define TEMPLATE template<template<class> class Field>
#include "field_field_functions_m.inc"


namespace mousse {

UNARY_FUNCTION(scalar, tensor, tr)
UNARY_FUNCTION(sphericalTensor, tensor, sph)
UNARY_FUNCTION(symmTensor, tensor, symm)
UNARY_FUNCTION(symmTensor, tensor, twoSymm)
UNARY_FUNCTION(tensor, tensor, skew)
UNARY_FUNCTION(tensor, tensor, dev)
UNARY_FUNCTION(tensor, tensor, dev2)
UNARY_FUNCTION(scalar, tensor, det)
UNARY_FUNCTION(tensor, tensor, cof)
UNARY_FUNCTION(tensor, tensor, inv)
UNARY_FUNCTION(vector, tensor, eigenValues)
UNARY_FUNCTION(tensor, tensor, eigenVectors)
UNARY_FUNCTION(vector, symmTensor, eigenValues)
UNARY_FUNCTION(symmTensor, symmTensor, eigenVectors)
// global operators 
UNARY_OPERATOR(vector, tensor, *, hdual)
UNARY_OPERATOR(tensor, vector, *, hdual)
BINARY_OPERATOR(vector, vector, tensor, /, divide)
BINARY_TYPE_OPERATOR(vector, vector, tensor, /, divide)

}  // namespace mousse

#include "undef_field_functions_m.inc"
#include "tensor_field_field.ipp"

#endif
