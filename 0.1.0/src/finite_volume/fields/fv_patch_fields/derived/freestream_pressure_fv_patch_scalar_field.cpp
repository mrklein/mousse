// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "freestream_pressure_fv_patch_scalar_field.hpp"
#include "freestream_fv_patch_fields.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Constructors 
mousse::freestreamPressureFvPatchScalarField::
freestreamPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  zeroGradientFvPatchScalarField(p, iF),
  UName_("U"),
  phiName_("phi"),
  rhoName_("rho")
{}
mousse::freestreamPressureFvPatchScalarField::
freestreamPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  zeroGradientFvPatchScalarField(p, iF, dict),
  UName_(dict.lookupOrDefault<word>("U", "U")),
  phiName_(dict.lookupOrDefault<word>("phi", "phi")),
  rhoName_(dict.lookupOrDefault<word>("rho", "rho"))
{}
mousse::freestreamPressureFvPatchScalarField::
freestreamPressureFvPatchScalarField
(
  const freestreamPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  zeroGradientFvPatchScalarField(ptf, p, iF, mapper),
  UName_(ptf.UName_),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_)
{}
mousse::freestreamPressureFvPatchScalarField::
freestreamPressureFvPatchScalarField
(
  const freestreamPressureFvPatchScalarField& wbppsf
)
:
  zeroGradientFvPatchScalarField(wbppsf),
  UName_(wbppsf.UName_),
  phiName_(wbppsf.phiName_),
  rhoName_(wbppsf.rhoName_)
{}
mousse::freestreamPressureFvPatchScalarField::
freestreamPressureFvPatchScalarField
(
  const freestreamPressureFvPatchScalarField& wbppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  zeroGradientFvPatchScalarField(wbppsf, iF),
  UName_(wbppsf.UName_),
  phiName_(wbppsf.phiName_),
  rhoName_(wbppsf.rhoName_)
{}
// Member Functions 
void mousse::freestreamPressureFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const freestreamFvPatchVectorField& Up =
    refCast<const freestreamFvPatchVectorField>
    (
      patch().lookupPatchField<volVectorField, vector>(UName_)
    );
  const surfaceScalarField& phi =
    db().lookupObject<surfaceScalarField>(phiName_);
  fvsPatchField<scalar>& phip =
    const_cast<fvsPatchField<scalar>&>
    (
      patch().patchField<surfaceScalarField, scalar>(phi)
    );
  if (phi.dimensions() == dimVelocity*dimArea)
  {
    phip = patch().Sf() & Up.freestreamValue();
  }
  else if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
  {
    const fvPatchField<scalar>& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    phip = rhop*(patch().Sf() & Up.freestreamValue());
  }
  else
  {
    FATAL_ERROR_IN_FUNCTION
      << "dimensions of phi are not correct"
      << "\n    on patch " << this->patch().name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << exit(FatalError);
  }
  zeroGradientFvPatchScalarField::updateCoeffs();
}
void mousse::freestreamPressureFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  writeEntryIfDifferent<word>(os, "U", "U", UName_);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  freestreamPressureFvPatchScalarField
);
}
