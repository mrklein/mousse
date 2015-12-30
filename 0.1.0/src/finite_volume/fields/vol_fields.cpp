// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"
namespace mousse
{
// Static Data Members
defineTemplate2TypeNameAndDebug(volScalarField::DimensionedInternalField, 0);
defineTemplate2TypeNameAndDebug(volVectorField::DimensionedInternalField, 0);
defineTemplate2TypeNameAndDebug
(
  volSphericalTensorField::DimensionedInternalField,
  0
);
defineTemplate2TypeNameAndDebug
(
  volSymmTensorField::DimensionedInternalField,
  0
);
defineTemplate2TypeNameAndDebug(volTensorField::DimensionedInternalField, 0);
defineTemplateTypeNameAndDebug(volScalarField, 0);
defineTemplateTypeNameAndDebug(volVectorField, 0);
defineTemplateTypeNameAndDebug(volSphericalTensorField, 0);
defineTemplateTypeNameAndDebug(volSymmTensorField, 0);
defineTemplateTypeNameAndDebug(volTensorField, 0);
// specialization for scalar fields
template<>
tmp<GeometricField<scalar, fvPatchField, volMesh> >
GeometricField<scalar, fvPatchField, volMesh>::component
(
  const direction
) const
{
  return *this;
}
// specialization for scalar fields
template<>
void GeometricField<scalar, fvPatchField, volMesh>::replace
(
  const direction,
  const GeometricField<scalar, fvPatchField, volMesh>& gsf
)
{
  *this == gsf;
}
}  // namespace mousse
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
