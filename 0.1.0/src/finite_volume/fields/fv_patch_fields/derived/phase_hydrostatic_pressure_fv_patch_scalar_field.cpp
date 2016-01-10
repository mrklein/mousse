// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_hydrostatic_pressure_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "uniform_dimensioned_fields.hpp"
// Constructors 
mousse::phaseHydrostaticPressureFvPatchScalarField::
phaseHydrostaticPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{p, iF},
  phaseName_{"alpha"},
  rho_{0.0},
  pRefValue_{0.0},
  pRefPoint_{vector::zero}
{
  this->refValue() = 0.0;
  this->refGrad() = 0.0;
  this->valueFraction() = 0.0;
}
mousse::phaseHydrostaticPressureFvPatchScalarField::
phaseHydrostaticPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchScalarField{p, iF},
  phaseName_{dict.lookupOrDefault<word>("phaseName", "alpha")},
  rho_{readScalar(dict.lookup("rho"))},
  pRefValue_{readScalar(dict.lookup("pRefValue"))},
  pRefPoint_{dict.lookup("pRefPoint")}
{
  this->refValue() = pRefValue_;
  if (dict.found("value"))
  {
    fvPatchScalarField::operator=
    (
      scalarField("value", dict, p.size())
    );
  }
  else
  {
    fvPatchScalarField::operator=(this->refValue());
  }
  this->refGrad() = 0.0;
  this->valueFraction() = 0.0;
}
mousse::phaseHydrostaticPressureFvPatchScalarField::
phaseHydrostaticPressureFvPatchScalarField
(
  const phaseHydrostaticPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchScalarField{ptf, p, iF, mapper},
  phaseName_{ptf.phaseName_},
  rho_{ptf.rho_},
  pRefValue_{ptf.pRefValue_},
  pRefPoint_{ptf.pRefPoint_}
{}
mousse::phaseHydrostaticPressureFvPatchScalarField::
phaseHydrostaticPressureFvPatchScalarField
(
  const phaseHydrostaticPressureFvPatchScalarField& ptf
)
:
  mixedFvPatchScalarField{ptf},
  phaseName_{ptf.phaseName_}
{}
mousse::phaseHydrostaticPressureFvPatchScalarField::
phaseHydrostaticPressureFvPatchScalarField
(
  const phaseHydrostaticPressureFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{ptf, iF},
  phaseName_{ptf.phaseName_},
  rho_{ptf.rho_},
  pRefValue_{ptf.pRefValue_},
  pRefPoint_{ptf.pRefPoint_}
{}
// Member Functions 
void mousse::phaseHydrostaticPressureFvPatchScalarField::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  const scalarField& alphap =
    patch().lookupPatchField<volScalarField, scalar>
    (
      phaseName_
    );
  const uniformDimensionedVectorField& g =
    db().lookupObject<uniformDimensionedVectorField>("g");
  // scalar rhor = 1000;
  // scalarField alphap1 = max(min(alphap, 1.0), 0.0);
  // valueFraction() = alphap1/(alphap1 + rhor*(1.0 - alphap1));
  valueFraction() = max(min(alphap, scalar(1.0)), scalar(0.0));
  refValue() =
    pRefValue_
   + rho_*((g.value() & patch().Cf()) - (g.value() & pRefPoint_));
  mixedFvPatchScalarField::updateCoeffs();
}
void mousse::phaseHydrostaticPressureFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  if (phaseName_ != "alpha")
  {
    os.writeKeyword("phaseName")
      << phaseName_ << token::END_STATEMENT << nl;
  }
  os.writeKeyword("rho") << rho_ << token::END_STATEMENT << nl;
  os.writeKeyword("pRefValue") << pRefValue_ << token::END_STATEMENT << nl;
  os.writeKeyword("pRefPoint") << pRefPoint_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
// Member Operators 
void mousse::phaseHydrostaticPressureFvPatchScalarField::operator=
(
  const fvPatchScalarField& ptf
)
{
  fvPatchScalarField::operator=
  (
    valueFraction()*refValue()
    + (1 - valueFraction())*ptf
  );
}

namespace mousse
{

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  phaseHydrostaticPressureFvPatchScalarField
);

}
