// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"


namespace mousse {

// Static Data Members
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  volScalarField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  volVectorField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  volSphericalTensorField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  volSymmTensorField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG
(
  volTensorField::DimensionedInternalField,
  0
);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(volScalarField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(volVectorField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(volSphericalTensorField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(volSymmTensorField, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(volTensorField, 0);

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
