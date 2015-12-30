// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_fixed_value_fv_patch_field.hpp"
// Constructors 
template<class Type>
mousse::uniformFixedValueFvPatchField<Type>::uniformFixedValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>(p, iF),
  uniformValue_()
{}
template<class Type>
mousse::uniformFixedValueFvPatchField<Type>::uniformFixedValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const Field<Type>& fld
)
:
  fixedValueFvPatchField<Type>(p, iF, fld),
  uniformValue_()
{}
template<class Type>
mousse::uniformFixedValueFvPatchField<Type>::uniformFixedValueFvPatchField
(
  const uniformFixedValueFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<Type>(p, iF),  // bypass mapper
  uniformValue_(ptf.uniformValue_().clone().ptr())
{
  // Evaluate since value not mapped
  const scalar t = this->db().time().timeOutputValue();
  fvPatchField<Type>::operator==(uniformValue_->value(t));
}
template<class Type>
mousse::uniformFixedValueFvPatchField<Type>::uniformFixedValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<Type>(p, iF),
  uniformValue_(DataEntry<Type>::New("uniformValue", dict))
{
  const scalar t = this->db().time().timeOutputValue();
  fvPatchField<Type>::operator==(uniformValue_->value(t));
}
template<class Type>
mousse::uniformFixedValueFvPatchField<Type>::uniformFixedValueFvPatchField
(
  const uniformFixedValueFvPatchField<Type>& ptf
)
:
  fixedValueFvPatchField<Type>(ptf),
  uniformValue_
  (
    ptf.uniformValue_.valid()
   ? ptf.uniformValue_().clone().ptr()
   : NULL
  )
{}
template<class Type>
mousse::uniformFixedValueFvPatchField<Type>::uniformFixedValueFvPatchField
(
  const uniformFixedValueFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>(ptf, iF),
  uniformValue_
  (
    ptf.uniformValue_.valid()
   ? ptf.uniformValue_().clone().ptr()
   : NULL
  )
{
  // For safety re-evaluate
  const scalar t = this->db().time().timeOutputValue();
  if (ptf.uniformValue_.valid())
  {
    fvPatchField<Type>::operator==(uniformValue_->value(t));
  }
}
// Member Functions 
template<class Type>
void mousse::uniformFixedValueFvPatchField<Type>::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  const scalar t = this->db().time().timeOutputValue();
  fvPatchField<Type>::operator==(uniformValue_->value(t));
  fixedValueFvPatchField<Type>::updateCoeffs();
}
template<class Type>
void mousse::uniformFixedValueFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  uniformValue_->writeData(os);
  this->writeEntry("value", os);
}
