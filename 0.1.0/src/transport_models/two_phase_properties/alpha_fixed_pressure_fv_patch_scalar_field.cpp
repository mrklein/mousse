// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "alpha_fixed_pressure_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "uniform_dimensioned_fields.hpp"
// Constructors 
mousse::alphaFixedPressureFvPatchScalarField::
alphaFixedPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(p, iF),
  p_(p.size(), 0.0)
{}
mousse::alphaFixedPressureFvPatchScalarField::
alphaFixedPressureFvPatchScalarField
(
  const alphaFixedPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField(ptf, p, iF, mapper),
  p_(ptf.p_, mapper)
{}
mousse::alphaFixedPressureFvPatchScalarField::
alphaFixedPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField(p, iF),
  p_("p", dict, p.size())
{
  if (dict.found("value"))
  {
    fvPatchField<scalar>::operator=
    (
      scalarField("value", dict, p.size())
    );
  }
  else
  {
    fvPatchField<scalar>::operator=(p_);
  }
}
mousse::alphaFixedPressureFvPatchScalarField::
alphaFixedPressureFvPatchScalarField
(
  const alphaFixedPressureFvPatchScalarField& tppsf
)
:
  fixedValueFvPatchScalarField(tppsf),
  p_(tppsf.p_)
{}
mousse::alphaFixedPressureFvPatchScalarField::
alphaFixedPressureFvPatchScalarField
(
  const alphaFixedPressureFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(tppsf, iF),
  p_(tppsf.p_)
{}
// Member Functions 
void mousse::alphaFixedPressureFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  scalarField::autoMap(m);
  p_.autoMap(m);
}
void mousse::alphaFixedPressureFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  fixedValueFvPatchScalarField::rmap(ptf, addr);
  const alphaFixedPressureFvPatchScalarField& tiptf =
    refCast<const alphaFixedPressureFvPatchScalarField>(ptf);
  p_.rmap(tiptf.p_, addr);
}
void mousse::alphaFixedPressureFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const uniformDimensionedVectorField& g =
    db().lookupObject<uniformDimensionedVectorField>("g");
  const fvPatchField<scalar>& rho =
    patch().lookupPatchField<volScalarField, scalar>("rho");
  operator==(p_ - rho*(g.value() & patch().Cf()));
  fixedValueFvPatchScalarField::updateCoeffs();
}
void mousse::alphaFixedPressureFvPatchScalarField::write
(
  Ostream& os
) const
{
  fvPatchScalarField::write(os);
  p_.writeEntry("p", os);
  writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  alphaFixedPressureFvPatchScalarField
);
}
