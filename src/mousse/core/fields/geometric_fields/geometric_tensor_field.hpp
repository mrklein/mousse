#ifndef CORE_FIELDS_GEOMETRIC_FIELDS_GEOMETRIC_TENSOR_FIELD_HPP_
#define CORE_FIELDS_GEOMETRIC_FIELDS_GEOMETRIC_TENSOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "geometric_spherical_tensor_field.hpp"
#include "geometric_symm_tensor_field.hpp"
#include "dimensioned_tensor_field.hpp"
#define TEMPLATE template<template<class> class PatchField, class GeoMesh>
#include "geometric_field_functions_m.inc"


namespace mousse {

UNARY_FUNCTION(tensor, tensor, T, transform)
UNARY_FUNCTION(scalar, tensor, tr, transform)
UNARY_FUNCTION(sphericalTensor, tensor, sph, transform)
UNARY_FUNCTION(symmTensor, tensor, symm, transform)
UNARY_FUNCTION(symmTensor, tensor, twoSymm, transform)
UNARY_FUNCTION(tensor, tensor, skew, transform)
UNARY_FUNCTION(tensor, tensor, dev, transform)
UNARY_FUNCTION(tensor, tensor, dev2, transform)
UNARY_FUNCTION(scalar, tensor, det, transform)
UNARY_FUNCTION(tensor, tensor, cof, cof)
UNARY_FUNCTION(tensor, tensor, inv, inv)
UNARY_FUNCTION(vector, tensor, eigenValues, transform)
UNARY_FUNCTION(tensor, tensor, eigenVectors, sign)
UNARY_FUNCTION(vector, symmTensor, eigenValues, transform)
UNARY_FUNCTION(symmTensor, symmTensor, eigenVectors, sign)

// global operators 
UNARY_OPERATOR(vector, tensor, *, hdual, transform)
UNARY_OPERATOR(tensor, vector, *, hdual, transform)
BINARY_OPERATOR(vector, vector, tensor, /, '|', divide)
BINARY_TYPE_OPERATOR(vector, vector, tensor, /, '|', divide)

}  // namespace mousse

#include "undef_field_functions_m.inc"
#include "geometric_tensor_field.ipp"

#endif
