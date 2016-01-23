// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "adjoint_outlet_pressure_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
// Constructors 
mousse::adjointOutletPressureFvPatchScalarField::
adjointOutletPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF}
{}
mousse::adjointOutletPressureFvPatchScalarField::
adjointOutletPressureFvPatchScalarField
(
  const adjointOutletPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper}
{}
mousse::adjointOutletPressureFvPatchScalarField::
adjointOutletPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF}
{
  fvPatchField<scalar>::operator=
  (
    scalarField("value", dict, p.size())
  );
}
mousse::adjointOutletPressureFvPatchScalarField::
adjointOutletPressureFvPatchScalarField
(
  const adjointOutletPressureFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{tppsf, iF}
{}
// Member Functions 
void mousse::adjointOutletPressureFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const fvsPatchField<scalar>& phip =
    patch().lookupPatchField<surfaceScalarField, scalar>("phi");
  const fvsPatchField<scalar>& phiap =
    patch().lookupPatchField<surfaceScalarField, scalar>("phia");
  const fvPatchField<vector>& Up =
    patch().lookupPatchField<volVectorField, vector>("U");
  const fvPatchField<vector>& Uap =
    patch().lookupPatchField<volVectorField, vector>("Ua");
  operator==((phiap/patch().magSf() - 1.0)*phip/patch().magSf() + (Up & Uap));
  fixedValueFvPatchScalarField::updateCoeffs();
}
void mousse::adjointOutletPressureFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  adjointOutletPressureFvPatchScalarField
);
}
