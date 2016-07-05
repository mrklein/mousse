// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_unburnt_enthalpy_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "psiu_reaction_thermo.hpp"


// Constructors 
mousse::fixedUnburntEnthalpyFvPatchScalarField::
fixedUnburntEnthalpyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF}
{}


mousse::fixedUnburntEnthalpyFvPatchScalarField::
fixedUnburntEnthalpyFvPatchScalarField
(
  const fixedUnburntEnthalpyFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper}
{}


mousse::fixedUnburntEnthalpyFvPatchScalarField::
fixedUnburntEnthalpyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF, dict}
{}


mousse::fixedUnburntEnthalpyFvPatchScalarField::
fixedUnburntEnthalpyFvPatchScalarField
(
  const fixedUnburntEnthalpyFvPatchScalarField& tppsf
)
:
  fixedValueFvPatchScalarField{tppsf}
{}


mousse::fixedUnburntEnthalpyFvPatchScalarField::
fixedUnburntEnthalpyFvPatchScalarField
(
  const fixedUnburntEnthalpyFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{tppsf, iF}
{}


// Member Functions 
void mousse::fixedUnburntEnthalpyFvPatchScalarField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const psiuReactionThermo& thermo =
    db().lookupObject<psiuReactionThermo>(basicThermo::dictName);
  const label patchi = patch().index();
  const scalarField& pw = thermo.p().boundaryField()[patchi];
  fvPatchScalarField& Tw =
    const_cast<fvPatchScalarField&>(thermo.Tu().boundaryField()[patchi]);
  Tw.evaluate();
  operator==(thermo.heu(pw, Tw, patchi));
  fixedValueFvPatchScalarField::updateCoeffs();
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  fixedUnburntEnthalpyFvPatchScalarField
);

}

