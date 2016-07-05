// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#include "energy_jump_fv_patch_scalar_field.hpp"
#include "fixed_jump_fv_patch_fields.hpp"
#include "basic_thermo.hpp"


// Constructors 

mousse::energyJumpFvPatchScalarField::energyJumpFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedJumpFvPatchField<scalar>{p, iF}
{}


mousse::energyJumpFvPatchScalarField::energyJumpFvPatchScalarField
(
  const energyJumpFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedJumpFvPatchField<scalar>{ptf, p, iF, mapper}
{}


mousse::energyJumpFvPatchScalarField::energyJumpFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedJumpFvPatchField<scalar>{p, iF}
{
  if (dict.found("value")) {
    fvPatchScalarField::operator=(scalarField{"value", dict, p.size()});
  } else {
    evaluate(Pstream::blocking);
  }
}


mousse::energyJumpFvPatchScalarField::energyJumpFvPatchScalarField
(
  const energyJumpFvPatchScalarField& ptf
)
:
  fixedJumpFvPatchField<scalar>{ptf}
{}


mousse::energyJumpFvPatchScalarField::energyJumpFvPatchScalarField
(
  const energyJumpFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedJumpFvPatchField<scalar>{ptf, iF}
{}


// Member Functions 
void mousse::energyJumpFvPatchScalarField::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  if (this->cyclicPatch().owner()) {
    const basicThermo& thermo = basicThermo::lookupThermo(*this);
    label patchID = patch().index();
    const scalarField& pp = thermo.p().boundaryField()[patchID];
    const fixedJumpFvPatchScalarField& TbPatch =
      refCast<const fixedJumpFvPatchScalarField>
      (
        thermo.T().boundaryField()[patchID]
      );
    fixedJumpFvPatchScalarField& Tbp =
      const_cast<fixedJumpFvPatchScalarField&>(TbPatch);
    // force update of jump
    Tbp.updateCoeffs();
    const labelUList& faceCells = this->patch().faceCells();
    jump_ = thermo.he(pp, Tbp.jump(), faceCells);
  }
  fixedJumpFvPatchField<scalar>::updateCoeffs();
}


void mousse::energyJumpFvPatchScalarField::write(Ostream& os) const
{
  fixedJumpFvPatchField<scalar>::write(os);
  this->writeEntry("value", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  energyJumpFvPatchScalarField
);

}

