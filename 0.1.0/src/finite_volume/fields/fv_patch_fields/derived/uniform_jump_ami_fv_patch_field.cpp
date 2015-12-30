// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_jump_ami_fv_patch_field.hpp"
// Constructors 
template<class Type>
mousse::uniformJumpAMIFvPatchField<Type>::uniformJumpAMIFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedJumpAMIFvPatchField<Type>(p, iF),
  jumpTable_(0)
{}
template<class Type>
mousse::uniformJumpAMIFvPatchField<Type>::uniformJumpAMIFvPatchField
(
  const uniformJumpAMIFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedJumpAMIFvPatchField<Type>(ptf, p, iF, mapper),
  jumpTable_(ptf.jumpTable_().clone().ptr())
{}
template<class Type>
mousse::uniformJumpAMIFvPatchField<Type>::uniformJumpAMIFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fixedJumpAMIFvPatchField<Type>(p, iF),
  jumpTable_(new DataEntry<Type>("jumpTable"))
{
  if (this->cyclicAMIPatch().owner())
  {
    jumpTable_ = DataEntry<Type>::New("jumpTable", dict);
  }
  if (dict.found("value"))
  {
    fvPatchField<Type>::operator=(Field<Type>("value", dict, p.size()));
  }
  else
  {
    this->evaluate(Pstream::blocking);
  }
}
template<class Type>
mousse::uniformJumpAMIFvPatchField<Type>::uniformJumpAMIFvPatchField
(
  const uniformJumpAMIFvPatchField<Type>& ptf
)
:
  fixedJumpAMIFvPatchField<Type>(ptf),
  jumpTable_(ptf.jumpTable_().clone().ptr())
{}
template<class Type>
mousse::uniformJumpAMIFvPatchField<Type>::uniformJumpAMIFvPatchField
(
  const uniformJumpAMIFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedJumpAMIFvPatchField<Type>(ptf, iF),
  jumpTable_(ptf.jumpTable_().clone().ptr())
{}
// Member Functions 
template<class Type>
void mousse::uniformJumpAMIFvPatchField<Type>::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  if (this->cyclicAMIPatch().owner())
  {
    this->jump_ = jumpTable_->value(this->db().time().value());
  }
  fixedJumpAMIFvPatchField<Type>::updateCoeffs();
}
template<class Type>
void mousse::uniformJumpAMIFvPatchField<Type>::write(Ostream& os) const
{
  fixedJumpAMIFvPatchField<Type>::write(os);
  if (this->cyclicAMIPatch().owner())
  {
    jumpTable_->writeData(os);
  }
}
