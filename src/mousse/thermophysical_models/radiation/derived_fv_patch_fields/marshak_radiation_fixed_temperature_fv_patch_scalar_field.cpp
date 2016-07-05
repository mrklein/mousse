// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "marshak_radiation_fixed_temperature_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "radiation_model.hpp"
#include "physico_chemical_constants.hpp"


// Constructors 
mousse::MarshakRadiationFixedTemperatureFvPatchScalarField::
MarshakRadiationFixedTemperatureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{p, iF},
  radiationCoupledBase{p, "undefined", scalarField::null()},
  Trad_{p.size()}
{
  refValue() = 0.0;
  refGrad() = 0.0;
  valueFraction() = 0.0;
}


mousse::MarshakRadiationFixedTemperatureFvPatchScalarField::
MarshakRadiationFixedTemperatureFvPatchScalarField
(
  const MarshakRadiationFixedTemperatureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchScalarField{ptf, p, iF, mapper},
  radiationCoupledBase{p, ptf.emissivityMethod(), ptf.emissivity_, mapper},
  Trad_{ptf.Trad_, mapper}
{}


mousse::MarshakRadiationFixedTemperatureFvPatchScalarField::
MarshakRadiationFixedTemperatureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchScalarField{p, iF},
  radiationCoupledBase{p, dict},
  Trad_{"Trad", dict, p.size()}
{
  using constant::physicoChemical::sigma;
  // refValue updated on each call to updateCoeffs()
  refValue() = 4.0*sigma.value()*pow4(Trad_);
  // zero gradient
  refGrad() = 0.0;
  valueFraction() = 1.0;
  fvPatchScalarField::operator=(refValue());
}


mousse::MarshakRadiationFixedTemperatureFvPatchScalarField::
MarshakRadiationFixedTemperatureFvPatchScalarField
(
  const MarshakRadiationFixedTemperatureFvPatchScalarField& ptf
)
:
  mixedFvPatchScalarField{ptf},
  radiationCoupledBase{ptf.patch(), ptf.emissivityMethod(), ptf.emissivity_},
  Trad_{ptf.Trad_}
{}


mousse::MarshakRadiationFixedTemperatureFvPatchScalarField::
MarshakRadiationFixedTemperatureFvPatchScalarField
(
  const MarshakRadiationFixedTemperatureFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{ptf, iF},
  radiationCoupledBase{ptf.patch(), ptf.emissivityMethod(), ptf.emissivity_},
  Trad_{ptf.Trad_}
{}


// Member Functions 
void mousse::MarshakRadiationFixedTemperatureFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  mixedFvPatchScalarField::autoMap(m);
  radiationCoupledBase::autoMap(m);
  Trad_.autoMap(m);
}


void mousse::MarshakRadiationFixedTemperatureFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  mixedFvPatchScalarField::rmap(ptf, addr);
  radiationCoupledBase::rmap(ptf, addr);
  const MarshakRadiationFixedTemperatureFvPatchScalarField& mrptf =
    refCast<const MarshakRadiationFixedTemperatureFvPatchScalarField>(ptf);
  Trad_.rmap(mrptf.Trad_, addr);
}


void mousse::MarshakRadiationFixedTemperatureFvPatchScalarField::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  // Since we're inside initEvaluate/evaluate there might be processor
  // comms underway. Change the tag we use.
  int oldTag = UPstream::msgType();
  UPstream::msgType() = oldTag+1;
  // Re-calc reference value
  refValue() = 4.0*constant::physicoChemical::sigma.value()*pow4(Trad_);
  // Diffusion coefficient - created by radiation model's ::updateCoeffs()
  const scalarField& gamma =
    patch().lookupPatchField<volScalarField, scalar>("gammaRad");
  const scalarField temissivity = emissivity();
  const scalarField Ep{temissivity/(2.0*(scalar(2.0) - temissivity))};
  // Set value fraction
  valueFraction() = 1.0/(1.0 + gamma*patch().deltaCoeffs()/Ep);
  // Restore tag
  UPstream::msgType() = oldTag;
  mixedFvPatchScalarField::updateCoeffs();
}


void mousse::MarshakRadiationFixedTemperatureFvPatchScalarField::write
(
  Ostream& os
) const
{
  mixedFvPatchScalarField::write(os);
  radiationCoupledBase::write(os);
  Trad_.writeEntry("Trad", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  MarshakRadiationFixedTemperatureFvPatchScalarField
);

}

