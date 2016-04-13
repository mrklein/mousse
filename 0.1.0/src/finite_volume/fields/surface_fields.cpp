// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_fields.hpp"


namespace mousse {

// Static Data Members
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  surfaceScalarField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  surfaceVectorField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  surfaceSphericalTensorField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  surfaceSymmTensorField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  surfaceTensorField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(surfaceScalarField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(surfaceVectorField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(surfaceSphericalTensorField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(surfaceSymmTensorField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(surfaceTensorField, 0);

}  // namespace mousse

