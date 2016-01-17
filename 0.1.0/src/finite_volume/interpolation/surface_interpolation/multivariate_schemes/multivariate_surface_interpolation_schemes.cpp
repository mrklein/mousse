// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multivariate_surface_interpolation_scheme.hpp"

namespace mousse
{

// Define the constructor function hash tables
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE
(
  multivariateSurfaceInterpolationScheme<scalar>,
  Istream
);

DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE
(
  multivariateSurfaceInterpolationScheme<vector>,
  Istream
);

DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE
(
  multivariateSurfaceInterpolationScheme<sphericalTensor>,
  Istream
);

DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE
(
  multivariateSurfaceInterpolationScheme<symmTensor>,
  Istream
);

DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE
(
  multivariateSurfaceInterpolationScheme<tensor>,
  Istream
);

}  // namespace mousse
