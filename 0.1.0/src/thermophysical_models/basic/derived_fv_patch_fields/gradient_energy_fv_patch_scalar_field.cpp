// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gradient_energy_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "basic_thermo.hpp"
// Constructors 
mousse::gradientEnergyFvPatchScalarField::
gradientEnergyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedGradientFvPatchScalarField(p, iF)
{}
mousse::gradientEnergyFvPatchScalarField::
gradientEnergyFvPatchScalarField
(
  const gradientEnergyFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedGradientFvPatchScalarField(ptf, p, iF, mapper)
{}
mousse::gradientEnergyFvPatchScalarField::
gradientEnergyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedGradientFvPatchScalarField(p, iF, dict)
{}
mousse::gradientEnergyFvPatchScalarField::
gradientEnergyFvPatchScalarField
(
  const gradientEnergyFvPatchScalarField& tppsf
)
:
  fixedGradientFvPatchScalarField(tppsf)
{}
mousse::gradientEnergyFvPatchScalarField::
gradientEnergyFvPatchScalarField
(
  const gradientEnergyFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedGradientFvPatchScalarField(tppsf, iF)
{}
// Member Functions 
void mousse::gradientEnergyFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const basicThermo& thermo = basicThermo::lookupThermo(*this);
  const label patchi = patch().index();
  const scalarField& pw = thermo.p().boundaryField()[patchi];
  fvPatchScalarField& Tw =
    const_cast<fvPatchScalarField&>(thermo.T().boundaryField()[patchi]);
  Tw.evaluate();
  gradient() = thermo.Cpv(pw, Tw, patchi)*Tw.snGrad()
   + patch().deltaCoeffs()*
    (
      thermo.he(pw, Tw, patchi)
     - thermo.he(pw, Tw, patch().faceCells())
    );
  fixedGradientFvPatchScalarField::updateCoeffs();
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchScalarField,
    gradientEnergyFvPatchScalarField
  );
}
