// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "swirl_flow_rate_inlet_velocity_fv_patch_vector_field.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "surface_fields.hpp"
#include "mathematical_constants.hpp"
// Constructors 
mousse::swirlFlowRateInletVelocityFvPatchVectorField::
swirlFlowRateInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>(p, iF),
  phiName_("phi"),
  rhoName_("rho"),
  flowRate_(),
  rpm_()
{}
mousse::swirlFlowRateInletVelocityFvPatchVectorField::
swirlFlowRateInletVelocityFvPatchVectorField
(
  const swirlFlowRateInletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<vector>(ptf, p, iF, mapper),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_),
  flowRate_(ptf.flowRate_().clone().ptr()),
  rpm_(ptf.rpm_().clone().ptr())
{}
mousse::swirlFlowRateInletVelocityFvPatchVectorField::
swirlFlowRateInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<vector>(p, iF, dict),
  phiName_(dict.lookupOrDefault<word>("phi", "phi")),
  rhoName_(dict.lookupOrDefault<word>("rho", "rho")),
  flowRate_(DataEntry<scalar>::New("flowRate", dict)),
  rpm_(DataEntry<scalar>::New("rpm", dict))
{}
mousse::swirlFlowRateInletVelocityFvPatchVectorField::
swirlFlowRateInletVelocityFvPatchVectorField
(
  const swirlFlowRateInletVelocityFvPatchVectorField& ptf
)
:
  fixedValueFvPatchField<vector>(ptf),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_),
  flowRate_(ptf.flowRate_().clone().ptr()),
  rpm_(ptf.rpm_().clone().ptr())
{}
mousse::swirlFlowRateInletVelocityFvPatchVectorField::
swirlFlowRateInletVelocityFvPatchVectorField
(
  const swirlFlowRateInletVelocityFvPatchVectorField& ptf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>(ptf, iF),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_),
  flowRate_(ptf.flowRate_().clone().ptr()),
  rpm_(ptf.rpm_().clone().ptr())
{}
// Member Functions 
void mousse::swirlFlowRateInletVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const scalar t = this->db().time().timeOutputValue();
  const scalar flowRate = flowRate_->value(t);
  const scalar rpm = rpm_->value(t);
  const scalar totArea   = gSum(patch().magSf());
  const scalar avgU = -flowRate/totArea;
  const vector avgCenter = gSum(patch().Cf()*patch().magSf())/totArea;
  const vector avgNormal = gSum(patch().Sf())/totArea;
  // Update angular velocity - convert [rpm] to [rad/s]
  tmp<vectorField> tangentialVelocity
    (
      (rpm*constant::mathematical::pi/30.0)
     * (patch().Cf() - avgCenter) ^ avgNormal
    );
  tmp<vectorField> n = patch().nf();
  const surfaceScalarField& phi =
    db().lookupObject<surfaceScalarField>(phiName_);
  if (phi.dimensions() == dimVelocity*dimArea)
  {
    // volumetric flow-rate
    operator==(tangentialVelocity + n*avgU);
  }
  else if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
  {
    const fvPatchField<scalar>& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    // mass flow-rate
    operator==(tangentialVelocity + n*avgU/rhop);
  }
  else
  {
    FatalErrorIn
    (
      "swirlFlowRateInletVelocityFvPatchVectorField::updateCoeffs()"
    )   << "dimensions of " << phiName_ << " are incorrect" << nl
      << "    on patch " << this->patch().name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << nl << exit(FatalError);
  }
  fixedValueFvPatchField<vector>::updateCoeffs();
}
void mousse::swirlFlowRateInletVelocityFvPatchVectorField::write
(
  Ostream& os
) const
{
  fvPatchField<vector>::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  flowRate_->writeData(os);
  rpm_->writeData(os);
  writeEntry("value", os);
}
namespace mousse
{
 makePatchTypeField
 (
   fvPatchVectorField,
   swirlFlowRateInletVelocityFvPatchVectorField
 );
}
