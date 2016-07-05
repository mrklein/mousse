// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "porous_baffle_pressure_fv_patch_field.hpp"
#include "surface_fields.hpp"
#include "turbulence_model.hpp"
#include "add_to_run_time_selection_table.hpp"


// Constructors 
mousse::porousBafflePressureFvPatchField::porousBafflePressureFvPatchField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedJumpFvPatchField<scalar>{p, iF},
  phiName_{"phi"},
  rhoName_{"rho"},
  D_{0},
  I_{0},
  length_{0}
{}


mousse::porousBafflePressureFvPatchField::porousBafflePressureFvPatchField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedJumpFvPatchField<scalar>{p, iF},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  rhoName_{dict.lookupOrDefault<word>("rho", "rho")},
  D_{readScalar(dict.lookup("D"))},
  I_{readScalar(dict.lookup("I"))},
  length_{readScalar(dict.lookup("length"))}
{
  fvPatchField<scalar>::operator=
  (
    Field<scalar>{"value", dict, p.size()}
  );
}


mousse::porousBafflePressureFvPatchField::porousBafflePressureFvPatchField
(
  const porousBafflePressureFvPatchField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedJumpFvPatchField<scalar>{ptf, p, iF, mapper},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_},
  D_{ptf.D_},
  I_{ptf.I_},
  length_{ptf.length_}
{}


mousse::porousBafflePressureFvPatchField::porousBafflePressureFvPatchField
(
  const porousBafflePressureFvPatchField& ptf
)
:
  cyclicLduInterfaceField(),
  fixedJumpFvPatchField<scalar>{ptf},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_},
  D_{ptf.D_},
  I_{ptf.I_},
  length_{ptf.length_}
{}


mousse::porousBafflePressureFvPatchField::porousBafflePressureFvPatchField
(
  const porousBafflePressureFvPatchField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedJumpFvPatchField<scalar>{ptf, iF},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_},
  D_{ptf.D_},
  I_{ptf.I_},
  length_{ptf.length_}
{}


// Member Functions 
void mousse::porousBafflePressureFvPatchField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const surfaceScalarField& phi =
    db().lookupObject<surfaceScalarField>(phiName_);
  const fvsPatchField<scalar>& phip =
    patch().patchField<surfaceScalarField, scalar>(phi);
  scalarField Un{phip/patch().magSf()};
  if (phi.dimensions() == dimDensity*dimVelocity*dimArea) {
    Un /= patch().lookupPatchField<volScalarField, scalar>(rhoName_);
  }
  scalarField magUn{mag(Un)};
  const turbulenceModel& turbModel =
    db().lookupObject<turbulenceModel>
    (
      IOobject::groupName
      (
        turbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  jump_ =
    -sign(Un)*(D_*turbModel.nu(patch().index()) + I_*0.5*magUn)*magUn*length_;
  if (dimensionedInternalField().dimensions() == dimPressure) {
    jump_ *= patch().lookupPatchField<volScalarField, scalar>(rhoName_);
  }
  if (debug) {
    scalar avePressureJump = gAverage(jump_);
    scalar aveVelocity = gAverage(mag(Un));
    Info << patch().boundaryMesh().mesh().name() << ':'
      << patch().name() << ':'
      << " Average pressure drop :" << avePressureJump
      << " Average velocity :" << aveVelocity
      << endl;
  }
  fixedJumpFvPatchField<scalar>::updateCoeffs();
}


void mousse::porousBafflePressureFvPatchField::write(Ostream& os) const
{
  fixedJumpFvPatchField<scalar>::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  os.writeKeyword("D") << D_ << token::END_STATEMENT << nl;
  os.writeKeyword("I") << I_ << token::END_STATEMENT << nl;
  os.writeKeyword("length") << length_ << token::END_STATEMENT << nl;
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  porousBafflePressureFvPatchField
);

}

