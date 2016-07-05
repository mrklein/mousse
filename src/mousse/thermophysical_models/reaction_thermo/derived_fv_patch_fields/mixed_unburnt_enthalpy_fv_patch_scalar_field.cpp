// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mixed_unburnt_enthalpy_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "psiu_reaction_thermo.hpp"


// Constructors 
mousse::mixedUnburntEnthalpyFvPatchScalarField::
mixedUnburntEnthalpyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{p, iF}
{
  valueFraction() = 0.0;
  refValue() = 0.0;
  refGrad() = 0.0;
}


mousse::mixedUnburntEnthalpyFvPatchScalarField::
mixedUnburntEnthalpyFvPatchScalarField
(
  const mixedUnburntEnthalpyFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchScalarField{ptf, p, iF, mapper}
{}


mousse::mixedUnburntEnthalpyFvPatchScalarField::
mixedUnburntEnthalpyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchScalarField{p, iF, dict}
{}


mousse::mixedUnburntEnthalpyFvPatchScalarField::
mixedUnburntEnthalpyFvPatchScalarField
(
  const mixedUnburntEnthalpyFvPatchScalarField& tppsf
)
:
  mixedFvPatchScalarField{tppsf}
{}


mousse::mixedUnburntEnthalpyFvPatchScalarField::
mixedUnburntEnthalpyFvPatchScalarField
(
  const mixedUnburntEnthalpyFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{tppsf, iF}
{}


// Member Functions 
void mousse::mixedUnburntEnthalpyFvPatchScalarField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const psiuReactionThermo& thermo =
    db().lookupObject<psiuReactionThermo>(basicThermo::dictName);
  const label patchi = patch().index();
  const scalarField& pw = thermo.p().boundaryField()[patchi];
  mixedFvPatchScalarField& Tw =
    refCast<mixedFvPatchScalarField>
    (
      const_cast<fvPatchScalarField&>(thermo.Tu().boundaryField()[patchi])
    );
  Tw.evaluate();
  valueFraction() = Tw.valueFraction();
  refValue() = thermo.heu(pw, Tw.refValue(), patchi);
  refGrad() = thermo.Cp(pw, Tw, patchi)*Tw.refGrad()
    + patch().deltaCoeffs()*(thermo.heu(pw, Tw, patchi)
                             - thermo.heu(pw, Tw, patch().faceCells()));
  mixedFvPatchScalarField::updateCoeffs();
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  mixedUnburntEnthalpyFvPatchScalarField
);

}

