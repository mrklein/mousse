// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef dimensioned_scalar_fwd_hpp_
#define dimensioned_scalar_fwd_hpp_
#include "scalar.hpp"
namespace mousse
{
template<class Type>
class dimensioned;
typedef dimensioned<scalar> dimensionedScalar;
}  // namespace mousse
#endif
