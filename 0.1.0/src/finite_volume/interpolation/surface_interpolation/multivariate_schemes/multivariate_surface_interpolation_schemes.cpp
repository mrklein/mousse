// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multivariate_surface_interpolation_scheme.hpp"
namespace mousse
{
// Define the constructor function hash tables
defineTemplateRunTimeSelectionTable
(
  multivariateSurfaceInterpolationScheme<scalar>,
  Istream
);
defineTemplateRunTimeSelectionTable
(
  multivariateSurfaceInterpolationScheme<vector>,
  Istream
);
defineTemplateRunTimeSelectionTable
(
  multivariateSurfaceInterpolationScheme<sphericalTensor>,
  Istream
);
defineTemplateRunTimeSelectionTable
(
  multivariateSurfaceInterpolationScheme<symmTensor>,
  Istream
);
defineTemplateRunTimeSelectionTable
(
  multivariateSurfaceInterpolationScheme<tensor>,
  Istream
);
}  // namespace mousse
