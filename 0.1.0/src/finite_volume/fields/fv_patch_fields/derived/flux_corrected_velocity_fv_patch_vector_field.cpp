// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "flux_corrected_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
// Constructors 
mousse::fluxCorrectedVelocityFvPatchVectorField::
fluxCorrectedVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  zeroGradientFvPatchVectorField(p, iF),
  phiName_("phi"),
  rhoName_("rho")
{}
mousse::fluxCorrectedVelocityFvPatchVectorField::
fluxCorrectedVelocityFvPatchVectorField
(
  const fluxCorrectedVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  zeroGradientFvPatchVectorField(ptf, p, iF, mapper),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_)
{}
mousse::fluxCorrectedVelocityFvPatchVectorField::
fluxCorrectedVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  zeroGradientFvPatchVectorField(p, iF),
  phiName_(dict.lookupOrDefault<word>("phi", "phi")),
  rhoName_(dict.lookupOrDefault<word>("rho", "rho"))
{
  fvPatchVectorField::operator=(patchInternalField());
}
mousse::fluxCorrectedVelocityFvPatchVectorField::
fluxCorrectedVelocityFvPatchVectorField
(
  const fluxCorrectedVelocityFvPatchVectorField& fcvpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  zeroGradientFvPatchVectorField(fcvpvf, iF),
  phiName_(fcvpvf.phiName_),
  rhoName_(fcvpvf.rhoName_)
{}
// Member Functions 
void mousse::fluxCorrectedVelocityFvPatchVectorField::evaluate
(
  const Pstream::commsTypes
)
{
  if (!updated())
  {
    updateCoeffs();
  }
  zeroGradientFvPatchVectorField::evaluate();
  const surfaceScalarField& phi =
    db().lookupObject<surfaceScalarField>(phiName_);
  const fvsPatchField<scalar>& phip =
    patch().patchField<surfaceScalarField, scalar>(phi);
  const vectorField n(patch().nf());
  const Field<scalar>& magS = patch().magSf();
  if (phi.dimensions() == dimVelocity*dimArea)
  {
    operator==(*this - n*(n & *this) + n*phip/magS);
  }
  else if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
  {
    const fvPatchField<scalar>& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    operator==(*this - n*(n & *this) + n*phip/(rhop*magS));
  }
  else
  {
    FATAL_ERROR_IN
    (
      "fluxCorrectedVelocityFvPatchVectorField::evaluate()"
    )
      << "dimensions of phi are incorrect\n"
      << "    on patch " << this->patch().name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << exit(FatalError);
  }
}
void mousse::fluxCorrectedVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  fluxCorrectedVelocityFvPatchVectorField
);
}
