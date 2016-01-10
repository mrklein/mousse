// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_fixed_value_point_patch_field.hpp"
namespace mousse
{
// Constructors
template<class Type>
uniformFixedValuePointPatchField<Type>::
uniformFixedValuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  fixedValuePointPatchField<Type>(p, iF),
  uniformValue_()
{}
template<class Type>
uniformFixedValuePointPatchField<Type>::
uniformFixedValuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchField<Type>(p, iF, dict, false),
  uniformValue_(DataEntry<Type>::New("uniformValue", dict))
{
  if (dict.found("value"))
  {
    fixedValuePointPatchField<Type>::operator==
    (
      Field<Type>("value", dict, p.size())
    );
  }
  else
  {
    const scalar t = this->db().time().timeOutputValue();
    fixedValuePointPatchField<Type>::operator=(uniformValue_->value(t));
  }
}
template<class Type>
uniformFixedValuePointPatchField<Type>::
uniformFixedValuePointPatchField
(
  const uniformFixedValuePointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<Type>(ptf, p, iF, mapper),
  uniformValue_(ptf.uniformValue_().clone().ptr())
{
  // For safety re-evaluate
  const scalar t = this->db().time().timeOutputValue();
  fixedValuePointPatchField<Type>::operator=(uniformValue_->value(t));
}
template<class Type>
uniformFixedValuePointPatchField<Type>::
uniformFixedValuePointPatchField
(
  const uniformFixedValuePointPatchField<Type>& ptf
)
:
  fixedValuePointPatchField<Type>(ptf),
  uniformValue_(ptf.uniformValue_().clone().ptr())
{}
template<class Type>
uniformFixedValuePointPatchField<Type>::
uniformFixedValuePointPatchField
(
  const uniformFixedValuePointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  fixedValuePointPatchField<Type>(ptf, iF),
  uniformValue_(ptf.uniformValue_().clone().ptr())
{
  // For safety re-evaluate
  const scalar t = this->db().time().timeOutputValue();
  fixedValuePointPatchField<Type>::operator==(uniformValue_->value(t));
}
// Member Functions 
template<class Type>
void uniformFixedValuePointPatchField<Type>::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  const scalar t = this->db().time().timeOutputValue();
  fixedValuePointPatchField<Type>::operator==(uniformValue_->value(t));
  fixedValuePointPatchField<Type>::updateCoeffs();
}
template<class Type>
void uniformFixedValuePointPatchField<Type>::
write(Ostream& os) const
{
  // Note: write value
  fixedValuePointPatchField<Type>::write(os);
  uniformValue_->writeData(os);
}
}  // namespace mousse
