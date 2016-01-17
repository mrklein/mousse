// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pressure_inlet_outlet_par_slip_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
// Constructors 
mousse::pressureInletOutletParSlipVelocityFvPatchVectorField::
pressureInletOutletParSlipVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  mixedFvPatchVectorField{p, iF},
  phiName_{"phi"},
  rhoName_{"rho"}
{
  refValue() = *this;
  refGrad() = vector::zero;
  valueFraction() = 0.0;
}
mousse::pressureInletOutletParSlipVelocityFvPatchVectorField::
pressureInletOutletParSlipVelocityFvPatchVectorField
(
  const pressureInletOutletParSlipVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchVectorField{ptf, p, iF, mapper},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_}
{}
mousse::pressureInletOutletParSlipVelocityFvPatchVectorField::
pressureInletOutletParSlipVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchVectorField{p, iF},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  rhoName_{dict.lookupOrDefault<word>("rho", "rho")}
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
  refValue() = *this;
  refGrad() = vector::zero;
  valueFraction() = 0.0;
}
mousse::pressureInletOutletParSlipVelocityFvPatchVectorField::
pressureInletOutletParSlipVelocityFvPatchVectorField
(
  const pressureInletOutletParSlipVelocityFvPatchVectorField& pivpvf
)
:
  mixedFvPatchVectorField{pivpvf},
  phiName_{pivpvf.phiName_},
  rhoName_{pivpvf.rhoName_}
{}
mousse::pressureInletOutletParSlipVelocityFvPatchVectorField::
pressureInletOutletParSlipVelocityFvPatchVectorField
(
  const pressureInletOutletParSlipVelocityFvPatchVectorField& pivpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  mixedFvPatchVectorField{pivpvf, iF},
  phiName_{pivpvf.phiName_},
  rhoName_{pivpvf.rhoName_}
{}
// Member Functions 
void mousse::pressureInletOutletParSlipVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const surfaceScalarField& phi =
    db().lookupObject<surfaceScalarField>(phiName_);
  const fvsPatchField<scalar>& phip =
    patch().patchField<surfaceScalarField, scalar>(phi);
  tmp<vectorField> n = patch().nf();
  const Field<scalar>& magSf = patch().magSf();
  // Get the tangential component from the internalField (zero-gradient)
  vectorField Ut(patchInternalField());
  Ut -= n()*(Ut & n());
  if (phi.dimensions() == dimVelocity*dimArea)
  {
    refValue() = Ut + n*phip/magSf;
  }
  else if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
  {
    const fvPatchField<scalar>& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    refValue() = Ut + n*phip/(rhop*magSf);
  }
  else
  {
    FATAL_ERROR_IN
    (
      "pressureInletOutletParSlipVelocityFvPatchVectorField::"
      "updateCoeffs()"
    )   << "dimensions of phi are not correct" << nl
      << "    on patch " << this->patch().name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << exit(FatalError);
  }
  valueFraction() = 1.0 - pos(phip);
  mixedFvPatchVectorField::updateCoeffs();
}
void mousse::pressureInletOutletParSlipVelocityFvPatchVectorField::write
(
  Ostream& os
) const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  writeEntry("value", os);
}
// Member Operators 
void mousse::pressureInletOutletParSlipVelocityFvPatchVectorField::operator=
(
  const fvPatchField<vector>& pvf
)
{
  fvPatchField<vector>::operator=(pvf);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  pressureInletOutletParSlipVelocityFvPatchVectorField
);
}
