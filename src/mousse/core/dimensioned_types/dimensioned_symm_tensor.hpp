#ifndef CORE_DIMENSIONED_TYPES_DIMENSIONED_SYMM_TENSOR_HPP_
#define CORE_DIMENSIONED_TYPES_DIMENSIONED_SYMM_TENSOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dimensioned_vector.hpp"
#include "symm_tensor.hpp"


namespace mousse {

typedef dimensioned<symmTensor> dimensionedSymmTensor;

// global functions
dimensionedSymmTensor sqr(const dimensionedVector&);
dimensionedSymmTensor innerSqr(const dimensionedSymmTensor&);
dimensionedScalar tr(const dimensionedSymmTensor&);
dimensionedSymmTensor symm(const dimensionedSymmTensor&);
dimensionedSymmTensor twoSymm(const dimensionedSymmTensor&);
dimensionedSymmTensor dev(const dimensionedSymmTensor&);
dimensionedSymmTensor dev2(const dimensionedSymmTensor&);
dimensionedScalar det(const dimensionedSymmTensor&);
dimensionedSymmTensor cof(const dimensionedSymmTensor&);
dimensionedSymmTensor inv(const dimensionedSymmTensor&);

// global operators
//- Hodge Dual operator (tensor -> vector)
dimensionedVector operator*(const dimensionedSymmTensor&);

}  // namespace mousse

#endif
