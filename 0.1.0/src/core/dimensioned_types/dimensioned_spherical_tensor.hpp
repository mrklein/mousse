// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   dimensioned_spherical_tensor.cpp
#ifndef dimensioned_spherical_tensor_hpp_
#define dimensioned_spherical_tensor_hpp_
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
