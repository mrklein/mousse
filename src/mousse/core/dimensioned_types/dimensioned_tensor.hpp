#ifndef CORE_DIMENSIONED_TYPES_DIMENSIONED_TENSOR_HPP_
#define CORE_DIMENSIONED_TYPES_DIMENSIONED_TENSOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dimensioned_vector.hpp"
#include "dimensioned_symm_tensor.hpp"
#include "tensor.hpp"


namespace mousse {

typedef dimensioned<tensor> dimensionedTensor;

// global functions
dimensionedScalar tr(const dimensionedTensor&);
dimensionedTensor dev(const dimensionedTensor&);
dimensionedTensor dev2(const dimensionedTensor&);
dimensionedScalar det(const dimensionedTensor&);
dimensionedTensor cof(const dimensionedTensor&);
dimensionedTensor inv(const dimensionedTensor&);
dimensionedSymmTensor symm(const dimensionedTensor&);
dimensionedSymmTensor twoSymm(const dimensionedTensor&);
dimensionedTensor skew(const dimensionedTensor&);
dimensionedVector eigenValues(const dimensionedTensor&);
dimensionedTensor eigenVectors(const dimensionedTensor&);
dimensionedVector eigenValues(const dimensionedSymmTensor&);
dimensionedTensor eigenVectors(const dimensionedSymmTensor&);

// global operators
//- Hodge Dual operator (tensor -> vector)
dimensionedVector operator*(const dimensionedTensor&);

//- Hodge Dual operator (vector -> tensor)
dimensionedTensor operator*(const dimensionedVector&);

}  // namespace mousse

#endif
