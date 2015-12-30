// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulent_intensity_kinetic_energy_inlet_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "surface_fields.hpp"
#include "vol_fields.hpp"
// Constructors 
mousse::turbulentIntensityKineticEnergyInletFvPatchScalarField::
turbulentIntensityKineticEnergyInletFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  inletOutletFvPatchScalarField(p, iF),
  intensity_(0.0),
  UName_("U")
{
  this->refValue() = 0.0;
  this->refGrad() = 0.0;
  this->valueFraction() = 0.0;
}
mousse::turbulentIntensityKineticEnergyInletFvPatchScalarField::
turbulentIntensityKineticEnergyInletFvPatchScalarField
(
  const turbulentIntensityKineticEnergyInletFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  inletOutletFvPatchScalarField(ptf, p, iF, mapper),
  intensity_(ptf.intensity_),
  UName_(ptf.UName_)
{}
mousse::turbulentIntensityKineticEnergyInletFvPatchScalarField::
turbulentIntensityKineticEnergyInletFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  inletOutletFvPatchScalarField(p, iF),
  intensity_(readScalar(dict.lookup("intensity"))),
  UName_(dict.lookupOrDefault<word>("U", "U"))
{
  this->phiName_ = dict.lookupOrDefault<word>("phi", "phi");
  if (intensity_ < 0 || intensity_ > 1)
  {
    FatalErrorIn
    (
      "turbulentIntensityKineticEnergyInletFvPatchScalarField::"
      "turbulentIntensityKineticEnergyInletFvPatchScalarField"
      "("
        "const fvPatch&, "
        "const DimensionedField<scalar, volMesh>&, "
        "const dictionary&"
      ")"
    )   << "Turbulence intensity should be specified as a fraction 0-1 "
       "of the mean velocity\n"
       "    value given is " << intensity_ << nl
      << "    on patch " << this->patch().name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << exit(FatalError);
  }
  fvPatchScalarField::operator=(scalarField("value", dict, p.size()));
  this->refValue() = 0.0;
  this->refGrad() = 0.0;
  this->valueFraction() = 0.0;
}
mousse::turbulentIntensityKineticEnergyInletFvPatchScalarField::
turbulentIntensityKineticEnergyInletFvPatchScalarField
(
  const turbulentIntensityKineticEnergyInletFvPatchScalarField& ptf
)
:
  inletOutletFvPatchScalarField(ptf),
  intensity_(ptf.intensity_),
  UName_(ptf.UName_)
{}
mousse::turbulentIntensityKineticEnergyInletFvPatchScalarField::
turbulentIntensityKineticEnergyInletFvPatchScalarField
(
  const turbulentIntensityKineticEnergyInletFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  inletOutletFvPatchScalarField(ptf, iF),
  intensity_(ptf.intensity_),
  UName_(ptf.UName_)
{}
// Member Functions 
void mousse::turbulentIntensityKineticEnergyInletFvPatchScalarField::
updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const fvPatchVectorField& Up =
    patch().lookupPatchField<volVectorField, vector>(UName_);
  const fvsPatchScalarField& phip =
    patch().lookupPatchField<surfaceScalarField, scalar>(this->phiName_);
  this->refValue() = 1.5*sqr(intensity_)*magSqr(Up);
  this->valueFraction() = 1.0 - pos(phip);
  inletOutletFvPatchScalarField::updateCoeffs();
}
void mousse::turbulentIntensityKineticEnergyInletFvPatchScalarField::write
(
  Ostream& os
) const
{
  fvPatchScalarField::write(os);
  os.writeKeyword("intensity") << intensity_ << token::END_STATEMENT << nl;
  writeEntryIfDifferent<word>(os, "U", "U", UName_);
  writeEntryIfDifferent<word>(os, "phi", "phi", this->phiName_);
  writeEntry("value", os);
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchScalarField,
    turbulentIntensityKineticEnergyInletFvPatchScalarField
  );
}
