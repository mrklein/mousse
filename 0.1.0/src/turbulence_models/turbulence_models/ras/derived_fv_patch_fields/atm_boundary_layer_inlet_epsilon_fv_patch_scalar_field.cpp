// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "atm_boundary_layer_inlet_epsilon_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
// Constructors 
atmBoundaryLayerInletEpsilonFvPatchScalarField::
atmBoundaryLayerInletEpsilonFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(p, iF),
  atmBoundaryLayer()
{}
atmBoundaryLayerInletEpsilonFvPatchScalarField::
atmBoundaryLayerInletEpsilonFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField(p, iF),
  atmBoundaryLayer(patch().Cf(), dict)
{
  scalarField::operator=(epsilon(patch().Cf()));
}
atmBoundaryLayerInletEpsilonFvPatchScalarField::
atmBoundaryLayerInletEpsilonFvPatchScalarField
(
  const atmBoundaryLayerInletEpsilonFvPatchScalarField& psf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField(psf, p, iF, mapper),
  atmBoundaryLayer(psf, mapper)
{}
atmBoundaryLayerInletEpsilonFvPatchScalarField::
atmBoundaryLayerInletEpsilonFvPatchScalarField
(
  const atmBoundaryLayerInletEpsilonFvPatchScalarField& psf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(psf, iF),
  atmBoundaryLayer(psf)
{}
// Member Functions 
void atmBoundaryLayerInletEpsilonFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedValueFvPatchScalarField::autoMap(m);
  atmBoundaryLayer::autoMap(m);
}
void atmBoundaryLayerInletEpsilonFvPatchScalarField::rmap
(
  const fvPatchScalarField& psf,
  const labelList& addr
)
{
  fixedValueFvPatchScalarField::rmap(psf, addr);
  const atmBoundaryLayerInletEpsilonFvPatchScalarField& blpsf =
    refCast<const atmBoundaryLayerInletEpsilonFvPatchScalarField>(psf);
  atmBoundaryLayer::rmap(blpsf, addr);
}
void atmBoundaryLayerInletEpsilonFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  atmBoundaryLayer::write(os);
  writeEntry("value", os);
}
makePatchTypeField
(
  fvPatchScalarField,
  atmBoundaryLayerInletEpsilonFvPatchScalarField
);
}  // namespace mousse
