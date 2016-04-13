#ifndef CORE_FIELDS_DIMENSIONED_FIELDS_DIMENSIONED_SYMM_TENSOR_FIELD_HPP_
#define CORE_FIELDS_DIMENSIONED_FIELDS_DIMENSIONED_SYMM_TENSOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dimensioned_field.hpp"
#include "symm_tensor.hpp"
#define TEMPLATE template<class GeoMesh>
#include "dimensioned_field_functions_m.inc"


namespace mousse {

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

#include "undef_field_functions_m.inc"
#include "dimensioned_symm_tensor_field.ipp"

#endif
