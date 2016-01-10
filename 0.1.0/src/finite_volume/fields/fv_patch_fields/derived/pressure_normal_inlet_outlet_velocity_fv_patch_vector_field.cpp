// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pressure_normal_inlet_outlet_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
// Constructors 
mousse::pressureNormalInletOutletVelocityFvPatchVectorField::
pressureNormalInletOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  mixedFvPatchVectorField(p, iF),
  phiName_("phi"),
  rhoName_("rho")
{
  refValue() = *this;
  refGrad() = vector::zero;
  valueFraction() = 0.0;
}
mousse::pressureNormalInletOutletVelocityFvPatchVectorField::
pressureNormalInletOutletVelocityFvPatchVectorField
(
  const pressureNormalInletOutletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchVectorField(ptf, p, iF, mapper),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_)
{}
mousse::pressureNormalInletOutletVelocityFvPatchVectorField::
pressureNormalInletOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchVectorField(p, iF),
  phiName_(dict.lookupOrDefault<word>("phi", "phi")),
  rhoName_(dict.lookupOrDefault<word>("rho", "rho"))
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
  refValue() = *this;
  refGrad() = vector::zero;
  valueFraction() = 0.0;
}
mousse::pressureNormalInletOutletVelocityFvPatchVectorField::
pressureNormalInletOutletVelocityFvPatchVectorField
(
  const pressureNormalInletOutletVelocityFvPatchVectorField& pivpvf
)
:
  mixedFvPatchVectorField(pivpvf),
  phiName_(pivpvf.phiName_),
  rhoName_(pivpvf.rhoName_)
{}
mousse::pressureNormalInletOutletVelocityFvPatchVectorField::
pressureNormalInletOutletVelocityFvPatchVectorField
(
  const pressureNormalInletOutletVelocityFvPatchVectorField& pivpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  mixedFvPatchVectorField(pivpvf, iF),
  phiName_(pivpvf.phiName_),
  rhoName_(pivpvf.rhoName_)
{}
// Member Functions 
void mousse::pressureNormalInletOutletVelocityFvPatchVectorField::updateCoeffs()
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
  const Field<scalar>& magS = patch().magSf();
  if (phi.dimensions() == dimVelocity*dimArea)
  {
    refValue() = n*phip/magS;
  }
  else if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
  {
    const fvPatchField<scalar>& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    refValue() = n*phip/(rhop*magS);
  }
  else
  {
    FATAL_ERROR_IN
    (
      "pressureNormalInletOutletVelocityFvPatchVectorField::"
      "updateCoeffs()"
    )   << "dimensions of phi are not correct"
      << "\n    on patch " << this->patch().name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << exit(FatalError);
  }
  valueFraction() = 1.0 - pos(phip);
  mixedFvPatchVectorField::updateCoeffs();
}
void mousse::pressureNormalInletOutletVelocityFvPatchVectorField::write
(
  Ostream& os
) const
{
  fvPatchVectorField::write(os);
  os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
  os.writeKeyword("rho") << rhoName_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
// Member Operators 
void mousse::pressureNormalInletOutletVelocityFvPatchVectorField::operator=
(
  const fvPatchField<vector>& pvf
)
{
  fvPatchField<vector>::operator=
  (
    valueFraction()*(patch().nf()*(patch().nf() & pvf))
   + (1 - valueFraction())*pvf
  );
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  pressureNormalInletOutletVelocityFvPatchVectorField
);
}
