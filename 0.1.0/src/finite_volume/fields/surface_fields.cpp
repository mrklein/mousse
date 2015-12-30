// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_fields.hpp"
namespace mousse
{
// Static Data Members
defineTemplate2TypeNameAndDebug
(
  surfaceScalarField::DimensionedInternalField,
  0
);
defineTemplate2TypeNameAndDebug
(
  surfaceVectorField::DimensionedInternalField,
  0
);
defineTemplate2TypeNameAndDebug
(
  surfaceSphericalTensorField::DimensionedInternalField,
  0
);
defineTemplate2TypeNameAndDebug
(
  surfaceSymmTensorField::DimensionedInternalField,
  0
);
defineTemplate2TypeNameAndDebug
(
  surfaceTensorField::DimensionedInternalField,
  0
);
defineTemplateTypeNameAndDebug(surfaceScalarField, 0);
defineTemplateTypeNameAndDebug(surfaceVectorField, 0);
defineTemplateTypeNameAndDebug(surfaceSphericalTensorField, 0);
defineTemplateTypeNameAndDebug(surfaceSymmTensorField, 0);
defineTemplateTypeNameAndDebug(surfaceTensorField, 0);
}  // namespace mousse
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
