// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multivariate_independent_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
defineNamedTemplateTypeNameAndDebug(multivariateIndependentScheme<scalar>, 0);
multivariateSurfaceInterpolationScheme<scalar>::addIstreamConstructorToTable
  <multivariateIndependentScheme<scalar> >
  addMultivariateIndependentSchemeScalarConstructorToTable_;
}  // namespace mousse
