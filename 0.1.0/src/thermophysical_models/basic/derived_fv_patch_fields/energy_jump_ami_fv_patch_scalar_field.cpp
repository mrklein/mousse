// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#include "energy_jump_ami_fv_patch_scalar_field.hpp"
#include "fixed_jump_ami_fv_patch_fields.hpp"
#include "basic_thermo.hpp"
// Constructors 
mousse::energyJumpAMIFvPatchScalarField::energyJumpAMIFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedJumpAMIFvPatchField<scalar>(p, iF)
{}
mousse::energyJumpAMIFvPatchScalarField::energyJumpAMIFvPatchScalarField
(
  const energyJumpAMIFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedJumpAMIFvPatchField<scalar>(ptf, p, iF, mapper)
{}
mousse::energyJumpAMIFvPatchScalarField::energyJumpAMIFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedJumpAMIFvPatchField<scalar>(p, iF)
{
  if (dict.found("value"))
  {
    fvPatchScalarField::operator=
    (
      scalarField("value", dict, p.size())
    );
  }
  else
  {
    evaluate(Pstream::blocking);
  }
}
mousse::energyJumpAMIFvPatchScalarField::energyJumpAMIFvPatchScalarField
(
  const energyJumpAMIFvPatchScalarField& ptf
)
:
  fixedJumpAMIFvPatchField<scalar>(ptf)
{}
mousse::energyJumpAMIFvPatchScalarField::energyJumpAMIFvPatchScalarField
(
  const energyJumpAMIFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedJumpAMIFvPatchField<scalar>(ptf, iF)
{}
// Member Functions 
void mousse::energyJumpAMIFvPatchScalarField::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  if (this->cyclicAMIPatch().owner())
  {
    const basicThermo& thermo = basicThermo::lookupThermo(*this);
    label patchID = patch().index();
    const scalarField& pp = thermo.p().boundaryField()[patchID];
    const fixedJumpAMIFvPatchScalarField& TbPatch =
      refCast<const fixedJumpAMIFvPatchScalarField>
      (
        thermo.T().boundaryField()[patchID]
      );
    fixedJumpAMIFvPatchScalarField& Tbp =
      const_cast<fixedJumpAMIFvPatchScalarField&>(TbPatch);
    // force update of jump
    Tbp.updateCoeffs();
    const labelUList& faceCells = this->patch().faceCells();
    jump_ = thermo.he(pp, Tbp.jump(), faceCells);
  }
  fixedJumpAMIFvPatchField<scalar>::updateCoeffs();
}
void mousse::energyJumpAMIFvPatchScalarField::write(Ostream& os) const
{
  fixedJumpAMIFvPatchField<scalar>::write(os);
  this->writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  energyJumpAMIFvPatchScalarField
);
}
