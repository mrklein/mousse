// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   geometric_spherical_tensor_field.cpp
#ifndef geometric_spherical_tensor_field_hpp_
#define geometric_spherical_tensor_field_hpp_
#include "geometric_field.hpp"
#include "spherical_tensor.hpp"
#define TEMPLATE template<template<class> class PatchField, class GeoMesh>
#include "geometric_field_functions_m.hpp"
namespace mousse
{
UNARY_FUNCTION(scalar, sphericalTensor, tr, transform)
UNARY_FUNCTION(sphericalTensor, sphericalTensor, sph, transform)
UNARY_FUNCTION(scalar, sphericalTensor, det, transform)
UNARY_FUNCTION(sphericalTensor, sphericalTensor, inv, inv)
BINARY_OPERATOR(sphericalTensor, scalar, sphericalTensor, /, '|', divide)
BINARY_TYPE_OPERATOR(sphericalTensor, scalar, sphericalTensor, /, '|', divide)
}  // namespace mousse
#include "undef_field_functions_m.hpp"
#ifdef NoRepository
#   include "geometric_spherical_tensor_field.cpp"
#endif
#endif