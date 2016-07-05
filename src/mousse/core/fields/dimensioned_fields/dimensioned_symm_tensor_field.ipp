// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dimensioned_symm_tensor_field.hpp"
#include "symm_tensor_field.hpp"
#define TEMPLATE template<class GeoMesh>
#include "dimensioned_field_functions_m.ipp"


namespace mousse {

UNARY_FUNCTION(symmTensor, vector, sqr, sqr)
UNARY_FUNCTION(symmTensor, symmTensor, innerSqr, sqr)
UNARY_FUNCTION(scalar, symmTensor, tr, transform)
UNARY_FUNCTION(sphericalTensor, symmTensor, sph, transform)
UNARY_FUNCTION(symmTensor, symmTensor, symm, transform)
UNARY_FUNCTION(symmTensor, symmTensor, twoSymm, transform)
UNARY_FUNCTION(symmTensor, symmTensor, dev, transform)
UNARY_FUNCTION(symmTensor, symmTensor, dev2, transform)
UNARY_FUNCTION(scalar, symmTensor, det, pow3)
UNARY_FUNCTION(symmTensor, symmTensor, cof, pow2)
UNARY_FUNCTION(symmTensor, symmTensor, inv, inv)
// global operators 
UNARY_OPERATOR(vector, symmTensor, *, hdual, transform)

}  // namespace mousse

#include "undef_field_functions_m.inc"
