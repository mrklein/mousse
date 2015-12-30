// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_jump_fv_patch_field.hpp"
// Constructors 
template<class Type>
mousse::uniformJumpFvPatchField<Type>::uniformJumpFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedJumpFvPatchField<Type>(p, iF),
  jumpTable_(new DataEntry<Type>("jumpTable"))
{}
template<class Type>
mousse::uniformJumpFvPatchField<Type>::uniformJumpFvPatchField
(
  const uniformJumpFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedJumpFvPatchField<Type>(ptf, p, iF, mapper),
  jumpTable_(ptf.jumpTable_().clone().ptr())
{}
template<class Type>
mousse::uniformJumpFvPatchField<Type>::uniformJumpFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fixedJumpFvPatchField<Type>(p, iF),
  jumpTable_(new DataEntry<Type>("jumpTable"))
{
  if (this->cyclicPatch().owner())
  {
    jumpTable_ = DataEntry<Type>::New("jumpTable", dict);
  }
  if (dict.found("value"))
  {
    fvPatchField<Type>::operator=
    (
      Field<Type>("value", dict, p.size())
    );
  }
  else
  {
    this->evaluate(Pstream::blocking);
  }
}
template<class Type>
mousse::uniformJumpFvPatchField<Type>::uniformJumpFvPatchField
(
  const uniformJumpFvPatchField<Type>& ptf
)
:
  fixedJumpFvPatchField<Type>(ptf),
  jumpTable_(ptf.jumpTable_().clone().ptr())
{}
template<class Type>
mousse::uniformJumpFvPatchField<Type>::uniformJumpFvPatchField
(
  const uniformJumpFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedJumpFvPatchField<Type>(ptf, iF),
  jumpTable_(ptf.jumpTable_().clone().ptr())
{}
// Member Functions 
template<class Type>
void mousse::uniformJumpFvPatchField<Type>::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  if (this->cyclicPatch().owner())
  {
    this->jump_ = jumpTable_->value(this->db().time().value());
  }
  fixedJumpFvPatchField<Type>::updateCoeffs();
}
template<class Type>
void mousse::uniformJumpFvPatchField<Type>::write(Ostream& os) const
{
  fixedJumpFvPatchField<Type>::write(os);
  if (this->cyclicPatch().owner())
  {
    jumpTable_->writeData(os);
  }
}
