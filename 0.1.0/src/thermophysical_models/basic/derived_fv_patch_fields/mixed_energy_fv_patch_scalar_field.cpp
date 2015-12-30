// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mixed_energy_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "basic_thermo.hpp"
// Constructors 
mousse::mixedEnergyFvPatchScalarField::
mixedEnergyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField(p, iF)
{
  valueFraction() = 0.0;
  refValue() = 0.0;
  refGrad() = 0.0;
}
mousse::mixedEnergyFvPatchScalarField::
mixedEnergyFvPatchScalarField
(
  const mixedEnergyFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchScalarField(ptf, p, iF, mapper)
{}
mousse::mixedEnergyFvPatchScalarField::
mixedEnergyFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchScalarField(p, iF, dict)
{}
mousse::mixedEnergyFvPatchScalarField::
mixedEnergyFvPatchScalarField
(
  const mixedEnergyFvPatchScalarField& tppsf
)
:
  mixedFvPatchScalarField(tppsf)
{}
mousse::mixedEnergyFvPatchScalarField::
mixedEnergyFvPatchScalarField
(
  const mixedEnergyFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField(tppsf, iF)
{}
// Member Functions 
void mousse::mixedEnergyFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const basicThermo& thermo = basicThermo::lookupThermo(*this);
  const label patchi = patch().index();
  const scalarField& pw = thermo.p().boundaryField()[patchi];
  mixedFvPatchScalarField& Tw = refCast<mixedFvPatchScalarField>
  (
    const_cast<fvPatchScalarField&>(thermo.T().boundaryField()[patchi])
  );
  Tw.evaluate();
  valueFraction() = Tw.valueFraction();
  refValue() = thermo.he(pw, Tw.refValue(), patchi);
  refGrad() =
    thermo.Cpv(pw, Tw, patchi)*Tw.refGrad()
   + patch().deltaCoeffs()*
    (
      thermo.he(pw, Tw, patchi)
     - thermo.he(pw, Tw, patch().faceCells())
    );
  mixedFvPatchScalarField::updateCoeffs();
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchScalarField,
    mixedEnergyFvPatchScalarField
  );
}
