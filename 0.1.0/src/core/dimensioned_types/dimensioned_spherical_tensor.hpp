#ifndef CORE_DIMENSIONED_TYPES_DIMENSIONED_SPHERICAL_TENSOR_HPP_
#define CORE_DIMENSIONED_TYPES_DIMENSIONED_SPHERICAL_TENSOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   dimensioned_spherical_tensor.cpp
#include "dimensioned_type.hpp"
#include "spherical_tensor.hpp"
namespace mousse
{
typedef dimensioned<sphericalTensor> dimensionedSphericalTensor;
// global functions
dimensionedScalar tr(const dimensionedSphericalTensor&);
dimensionedScalar det(const dimensionedSphericalTensor&);
dimensionedSphericalTensor inv(const dimensionedSphericalTensor&);
}  // namespace mousse
#endif
