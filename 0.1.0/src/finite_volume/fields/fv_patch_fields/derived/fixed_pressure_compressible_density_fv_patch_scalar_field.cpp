// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_pressure_compressible_density_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "surface_fields.hpp"
#include "vol_fields.hpp"
// Constructors 
mousse::fixedPressureCompressibleDensityFvPatchScalarField::
fixedPressureCompressibleDensityFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchField<scalar>(p, iF),
  pName_("p")
{}
mousse::fixedPressureCompressibleDensityFvPatchScalarField::
fixedPressureCompressibleDensityFvPatchScalarField
(
  const fixedPressureCompressibleDensityFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<scalar>(ptf, p, iF, mapper),
  pName_(ptf.pName_)
{}
mousse::fixedPressureCompressibleDensityFvPatchScalarField::
fixedPressureCompressibleDensityFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<scalar>(p, iF, dict),
  pName_(dict.lookupOrDefault<word>("p", "p"))
{}
mousse::fixedPressureCompressibleDensityFvPatchScalarField::
fixedPressureCompressibleDensityFvPatchScalarField
(
  const fixedPressureCompressibleDensityFvPatchScalarField& ptf
)
:
  fixedValueFvPatchField<scalar>(ptf),
  pName_(ptf.pName_)
{}
mousse::fixedPressureCompressibleDensityFvPatchScalarField::
fixedPressureCompressibleDensityFvPatchScalarField
(
  const fixedPressureCompressibleDensityFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchField<scalar>(ptf, iF),
  pName_(ptf.pName_)
{}
// Member Functions 
void mousse::fixedPressureCompressibleDensityFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const fvPatchField<scalar>& pp =
    patch().lookupPatchField<volScalarField, scalar>(pName_);
  const dictionary& thermoProps =
    db().lookupObject<IOdictionary>("thermodynamicProperties");
  const scalar rholSat =
    dimensionedScalar(thermoProps.lookup("rholSat")).value();
  const scalar pSat =
    dimensionedScalar(thermoProps.lookup("pSat")).value();
  const scalar psil = dimensionedScalar(thermoProps.lookup("psil")).value();
  operator==(rholSat + psil*(pp - pSat));
  fixedValueFvPatchField<scalar>::updateCoeffs();
}
void mousse::fixedPressureCompressibleDensityFvPatchScalarField::write
(
  Ostream& os
) const
{
  fvPatchField<scalar>::write(os);
  writeEntryIfDifferent<word>(os, "p", "p", pName_);
  writeEntry("value", os);
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchScalarField,
    fixedPressureCompressibleDensityFvPatchScalarField
  );
}
