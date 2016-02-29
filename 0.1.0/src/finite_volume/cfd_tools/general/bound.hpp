#ifndef FINITE_VOLUME_CFD_TOOLS_GENERAL_BOUND_HPP_
#define FINITE_VOLUME_CFD_TOOLS_GENERAL_BOUND_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   bound.cpp


#include "dimensioned_scalar.hpp"
#include "vol_fields_fwd.hpp"

namespace mousse
{
// Global Functions 
//- Bound the given scalar field if it has gone unbounded.
//  Return the bounded field.
//  Used extensively in RAS and LES turbulence models.
volScalarField& bound(volScalarField&, const dimensionedScalar& lowerBound);
}  // namespace mousse

#endif
