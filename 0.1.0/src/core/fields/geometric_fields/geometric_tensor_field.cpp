// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "geometric_tensor_field.hpp"
#include "tensor_field_field.hpp"
#define TEMPLATE template<template<class> class PatchField, class GeoMesh>
#include "geometric_field_functions_m.cpp"
namespace mousse
{
UNARY_FUNCTION(tensor, tensor, T, transform)
UNARY_FUNCTION(scalar, tensor, tr, transform)
UNARY_FUNCTION(sphericalTensor, tensor, sph, transform)
UNARY_FUNCTION(symmTensor, tensor, symm, transform)
UNARY_FUNCTION(symmTensor, tensor, twoSymm, transform)
UNARY_FUNCTION(tensor, tensor, skew, transform)
UNARY_FUNCTION(tensor, tensor, dev, transform)
UNARY_FUNCTION(tensor, tensor, dev2, transform)
UNARY_FUNCTION(scalar, tensor, det, pow3)
UNARY_FUNCTION(tensor, tensor, cof, pow2)
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
#include "undef_field_functions_m.hpp"
