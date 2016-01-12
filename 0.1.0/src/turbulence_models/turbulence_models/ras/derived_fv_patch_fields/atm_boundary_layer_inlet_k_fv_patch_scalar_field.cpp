// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "atm_boundary_layer_inlet_k_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
// Constructors 
atmBoundaryLayerInletKFvPatchScalarField::
atmBoundaryLayerInletKFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF},
  atmBoundaryLayer{}
{}
atmBoundaryLayerInletKFvPatchScalarField::
atmBoundaryLayerInletKFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF},
  atmBoundaryLayer{patch().Cf(), dict}
{
  scalarField::operator=(k(patch().Cf()));
}
atmBoundaryLayerInletKFvPatchScalarField::
atmBoundaryLayerInletKFvPatchScalarField
(
  const atmBoundaryLayerInletKFvPatchScalarField& psf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{psf, p, iF, mapper},
  atmBoundaryLayer{psf, mapper}
{}
atmBoundaryLayerInletKFvPatchScalarField::
atmBoundaryLayerInletKFvPatchScalarField
(
  const atmBoundaryLayerInletKFvPatchScalarField& psf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{psf, iF},
  atmBoundaryLayer{psf}
{}
// Member Functions 
void atmBoundaryLayerInletKFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedValueFvPatchScalarField::autoMap(m);
  atmBoundaryLayer::autoMap(m);
}
void atmBoundaryLayerInletKFvPatchScalarField::rmap
(
  const fvPatchScalarField& psf,
  const labelList& addr
)
{
  fixedValueFvPatchScalarField::rmap(psf, addr);
  const atmBoundaryLayerInletKFvPatchScalarField& blpsf =
    refCast<const atmBoundaryLayerInletKFvPatchScalarField>(psf);
  atmBoundaryLayer::rmap(blpsf, addr);
}
void atmBoundaryLayerInletKFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  atmBoundaryLayer::write(os);
  writeEntry("value", os);
}
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  atmBoundaryLayerInletKFvPatchScalarField
);
}  // namespace mousse
