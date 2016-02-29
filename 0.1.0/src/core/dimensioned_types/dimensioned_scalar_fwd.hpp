#ifndef CORE_DIMENSIONED_TYPES_DIMENSIONED_SCALAR_FWD_HPP_
#define CORE_DIMENSIONED_TYPES_DIMENSIONED_SCALAR_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar.hpp"
namespace mousse
{
template<class Type>
class dimensioned;
typedef dimensioned<scalar> dimensionedScalar;
}  // namespace mousse
#endif
