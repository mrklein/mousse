// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multivariate_selection_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


namespace mousse {

DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  multivariateSelectionScheme<scalar>,
  0
);

multivariateSurfaceInterpolationScheme<scalar>::addIstreamConstructorToTable
  <multivariateSelectionScheme<scalar> >
  addMultivariateSelectionSchemeScalarConstructorToTable_;

}  // namespace mousse

