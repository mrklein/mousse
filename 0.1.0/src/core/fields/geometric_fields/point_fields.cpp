// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "point_fields.hpp"

namespace mousse
{

// Static Data Members
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  pointScalarField::DimensionedInternalField, 0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  pointVectorField::DimensionedInternalField, 0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  pointSphericalTensorField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  pointSymmTensorField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  pointTensorField::DimensionedInternalField, 0
);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(pointScalarField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(pointVectorField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(pointSphericalTensorField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(pointSymmTensorField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(pointTensorField, 0);

}  // namespace mousse
