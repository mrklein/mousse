#ifndef CORE_FIELDS_GEOMETRIC_FIELDS_GEOMETRIC_SYMM_TENSOR_FIELD_HPP_
#define CORE_FIELDS_GEOMETRIC_FIELDS_GEOMETRIC_SYMM_TENSOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   geometric_symm_tensor_field.cpp
#include "geometric_field.hpp"
#include "dimensioned_symm_tensor_field.hpp"
#define TEMPLATE template<template<class> class PatchField, class GeoMesh>
#include "geometric_field_functions_m.hpp"
namespace mousse
{
UNARY_FUNCTION(symmTensor, vector, sqr, sqr)
UNARY_FUNCTION(symmTensor, symmTensor, innerSqr, sqr)
UNARY_FUNCTION(scalar, symmTensor, tr, transform)
UNARY_FUNCTION(sphericalTensor, symmTensor, sph, transform)
UNARY_FUNCTION(symmTensor, symmTensor, symm, transform)
UNARY_FUNCTION(symmTensor, symmTensor, twoSymm, transform)
UNARY_FUNCTION(symmTensor, symmTensor, dev, transform)
UNARY_FUNCTION(symmTensor, symmTensor, dev2, transform)
UNARY_FUNCTION(scalar, symmTensor, det, transform)
UNARY_FUNCTION(symmTensor, symmTensor, cof, cof)
UNARY_FUNCTION(symmTensor, symmTensor, inv, inv)
// global operators 
UNARY_OPERATOR(vector, symmTensor, *, hdual, transform)
BINARY_OPERATOR(tensor, symmTensor, symmTensor, &, '&', dot)
BINARY_TYPE_OPERATOR(tensor, symmTensor, symmTensor, &, '&', dot)
}  // namespace mousse
#include "undef_field_functions_m.hpp"
#ifdef NoRepository
#   include "geometric_symm_tensor_field.cpp"
#endif
#endif
