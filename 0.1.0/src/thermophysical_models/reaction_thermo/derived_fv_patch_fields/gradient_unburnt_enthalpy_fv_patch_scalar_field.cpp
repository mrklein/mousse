// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gradient_unburnt_enthalpy_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "psiu_reaction_thermo.hpp"
// Constructors 
mousse::gradientUnburntEnthalpyFvPatchScalarField::
gradientUnburntEnthalpyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedGradientFvPatchScalarField{p, iF}
{}
mousse::gradientUnburntEnthalpyFvPatchScalarField::
gradientUnburntEnthalpyFvPatchScalarField
(
  const gradientUnburntEnthalpyFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedGradientFvPatchScalarField{ptf, p, iF, mapper}
{}
mousse::gradientUnburntEnthalpyFvPatchScalarField::
gradientUnburntEnthalpyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedGradientFvPatchScalarField{p, iF, dict}
{}
mousse::gradientUnburntEnthalpyFvPatchScalarField::
gradientUnburntEnthalpyFvPatchScalarField
(
  const gradientUnburntEnthalpyFvPatchScalarField& tppsf
)
:
  fixedGradientFvPatchScalarField{tppsf}
{}
mousse::gradientUnburntEnthalpyFvPatchScalarField::
gradientUnburntEnthalpyFvPatchScalarField
(
  const gradientUnburntEnthalpyFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedGradientFvPatchScalarField{tppsf, iF}
{}
// Member Functions 
void mousse::gradientUnburntEnthalpyFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const psiuReactionThermo& thermo = db().lookupObject<psiuReactionThermo>
  (
    basicThermo::dictName
  );
  const label patchi = patch().index();
  const scalarField& pw = thermo.p().boundaryField()[patchi];
  fvPatchScalarField& Tw =
    const_cast<fvPatchScalarField&>(thermo.Tu().boundaryField()[patchi]);
  Tw.evaluate();
  gradient() = thermo.Cp(pw, Tw, patchi)*Tw.snGrad()
   + patch().deltaCoeffs()*
    (
      thermo.heu(pw, Tw, patchi)
     - thermo.heu(pw, Tw, patch().faceCells())
    );
  fixedGradientFvPatchScalarField::updateCoeffs();
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  gradientUnburntEnthalpyFvPatchScalarField
);
}
