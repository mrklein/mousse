// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_fields.hpp"
#include "limited_scheme.hpp"
#include "filtered_linear.hpp"

namespace mousse
{

MAKE_LIMITED_SURFACE_INTERPOLATION_SCHEME
(
  filteredLinear,
  filteredLinearLimiter
)

}
