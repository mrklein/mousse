// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   dimensioned_symm_tensor_field.cpp
#ifndef dimensioned_symm_tensor_field_hpp_
#define dimensioned_symm_tensor_field_hpp_
#include "dimensioned_field.hpp"
#include "symm_tensor.hpp"
#define TEMPLATE template<class GeoMesh>
#include "dimensioned_field_functions_m.hpp"
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
}  // namespace mousse
#include "undef_field_functions_m.hpp"
#ifdef NoRepository
#   include "dimensioned_symm_tensor_field.cpp"
#endif
#endif
