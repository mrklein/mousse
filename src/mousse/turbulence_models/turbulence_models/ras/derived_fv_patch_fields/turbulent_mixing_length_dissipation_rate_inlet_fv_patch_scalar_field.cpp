// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulent_mixing_length_dissipation_rate_inlet_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "surface_fields.hpp"
#include "vol_fields.hpp"
#include "turbulence_model.hpp"


namespace mousse {

// Constructors 
turbulentMixingLengthDissipationRateInletFvPatchScalarField::
turbulentMixingLengthDissipationRateInletFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  inletOutletFvPatchScalarField{p, iF},
  mixingLength_{0.0},
  kName_{"k"}
{
  this->refValue() = 0.0;
  this->refGrad() = 0.0;
  this->valueFraction() = 0.0;
}


turbulentMixingLengthDissipationRateInletFvPatchScalarField::
turbulentMixingLengthDissipationRateInletFvPatchScalarField
(
  const turbulentMixingLengthDissipationRateInletFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  inletOutletFvPatchScalarField{ptf, p, iF, mapper},
  mixingLength_{ptf.mixingLength_},
  kName_{ptf.kName_}
{}


turbulentMixingLengthDissipationRateInletFvPatchScalarField::
turbulentMixingLengthDissipationRateInletFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  inletOutletFvPatchScalarField{p, iF},
  mixingLength_{readScalar(dict.lookup("mixingLength"))},
  kName_{dict.lookupOrDefault<word>("k", "k")}
{
  this->phiName_ = dict.lookupOrDefault<word>("phi", "phi");
  fvPatchScalarField::operator=(scalarField{"value", dict, p.size()});
  this->refValue() = 0.0;
  this->refGrad() = 0.0;
  this->valueFraction() = 0.0;
}


turbulentMixingLengthDissipationRateInletFvPatchScalarField::
turbulentMixingLengthDissipationRateInletFvPatchScalarField
(
  const turbulentMixingLengthDissipationRateInletFvPatchScalarField& ptf
)
:
  inletOutletFvPatchScalarField{ptf},
  mixingLength_{ptf.mixingLength_},
  kName_{ptf.kName_}
{}


turbulentMixingLengthDissipationRateInletFvPatchScalarField::
turbulentMixingLengthDissipationRateInletFvPatchScalarField
(
  const turbulentMixingLengthDissipationRateInletFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  inletOutletFvPatchScalarField{ptf, iF},
  mixingLength_{ptf.mixingLength_},
  kName_{ptf.kName_}
{}


// Member Functions 
void turbulentMixingLengthDissipationRateInletFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  // Lookup Cmu corresponding to the turbulence model selected
  const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
  (
    IOobject::groupName
    (
      turbulenceModel::propertiesName,
      dimensionedInternalField().group()
    )
  );
  const scalar Cmu =
    turbModel.coeffDict().lookupOrDefault<scalar>("Cmu", 0.09);
  const scalar Cmu75 = pow(Cmu, 0.75);
  const fvPatchScalarField& kp =
    patch().lookupPatchField<volScalarField, scalar>(kName_);
  const fvsPatchScalarField& phip =
    patch().lookupPatchField<surfaceScalarField, scalar>(this->phiName_);
  this->refValue() = Cmu75*kp*sqrt(kp)/mixingLength_;
  this->valueFraction() = 1.0 - pos(phip);
  inletOutletFvPatchScalarField::updateCoeffs();
}


void turbulentMixingLengthDissipationRateInletFvPatchScalarField::write
(
  Ostream& os
) const
{
  fvPatchScalarField::write(os);
  os.writeKeyword("mixingLength")
    << mixingLength_ << token::END_STATEMENT << nl;
  os.writeKeyword("phi") << this->phiName_ << token::END_STATEMENT << nl;
  os.writeKeyword("k") << kName_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  turbulentMixingLengthDissipationRateInletFvPatchScalarField
);

}  // namespace mousse

