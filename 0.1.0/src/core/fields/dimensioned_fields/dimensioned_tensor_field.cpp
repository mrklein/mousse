// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dimensioned_tensor_field.hpp"
#include "tensor_field.hpp"
#define TEMPLATE template<class GeoMesh>
#include "dimensioned_field_functions_m.cpp"
namespace mousse
{
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
}  // namespace mousse
#include "undef_field_functions_m.hpp"
