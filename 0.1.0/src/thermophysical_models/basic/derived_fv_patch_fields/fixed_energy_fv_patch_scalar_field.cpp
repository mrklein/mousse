// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "basic_thermo.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fixed_energy_fv_patch_scalar_field.hpp"
// Constructors 
mousse::fixedEnergyFvPatchScalarField::
fixedEnergyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(p, iF)
{}
mousse::fixedEnergyFvPatchScalarField::
fixedEnergyFvPatchScalarField
(
  const fixedEnergyFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField(ptf, p, iF, mapper)
{}
mousse::fixedEnergyFvPatchScalarField::
fixedEnergyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField(p, iF, dict)
{}
mousse::fixedEnergyFvPatchScalarField::
fixedEnergyFvPatchScalarField
(
  const fixedEnergyFvPatchScalarField& tppsf
)
:
  fixedValueFvPatchScalarField(tppsf)
{}
mousse::fixedEnergyFvPatchScalarField::
fixedEnergyFvPatchScalarField
(
  const fixedEnergyFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(tppsf, iF)
{}
// Member Functions 
void mousse::fixedEnergyFvPatchScalarField::updateCoeffs()
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
  operator==(thermo.he(pw, Tw, patchi));
  fixedValueFvPatchScalarField::updateCoeffs();
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  fixedEnergyFvPatchScalarField
);
}
