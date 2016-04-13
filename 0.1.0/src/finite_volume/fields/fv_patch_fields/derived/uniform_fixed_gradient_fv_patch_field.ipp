// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_fixed_gradient_fv_patch_field.hpp"


// Constructors 
template<class Type>
mousse::uniformFixedGradientFvPatchField<Type>::uniformFixedGradientFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedGradientFvPatchField<Type>{p, iF},
  uniformGradient_{}
{}


template<class Type>
mousse::uniformFixedGradientFvPatchField<Type>::uniformFixedGradientFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const Field<Type>& fld
)
:
  fixedGradientFvPatchField<Type>{p, iF, fld},
  uniformGradient_{}
{}


template<class Type>
mousse::uniformFixedGradientFvPatchField<Type>::uniformFixedGradientFvPatchField
(
  const uniformFixedGradientFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedGradientFvPatchField<Type>{ptf, p, iF, mapper},
  uniformGradient_{ptf.uniformGradient_().clone().ptr()}
{
  // For safety re-evaluate
  const scalar t = this->db().time().timeOutputValue();
  this->gradient() = uniformGradient_->value(t);
}


template<class Type>
mousse::uniformFixedGradientFvPatchField<Type>::uniformFixedGradientFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fixedGradientFvPatchField<Type>{p, iF},
  uniformGradient_{DataEntry<Type>::New("uniformGradient", dict)}
{
  if (dict.found("gradient")) {
    this->gradient() = Field<Type>("gradient", dict, p.size());
  } else {
    const scalar t = this->db().time().timeOutputValue();
    this->gradient() = uniformGradient_->value(t);
  }
  fixedGradientFvPatchField<Type>::evaluate();
}


template<class Type>
mousse::uniformFixedGradientFvPatchField<Type>::uniformFixedGradientFvPatchField
(
  const uniformFixedGradientFvPatchField<Type>& ptf
)
:
  fixedGradientFvPatchField<Type>{ptf},
  uniformGradient_
  {
    ptf.uniformGradient_.valid()
    ? ptf.uniformGradient_().clone().ptr()
    : NULL
  }
{}


template<class Type>
mousse::uniformFixedGradientFvPatchField<Type>::uniformFixedGradientFvPatchField
(
  const uniformFixedGradientFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedGradientFvPatchField<Type>{ptf, iF},
  uniformGradient_
  {
    ptf.uniformGradient_.valid()
    ? ptf.uniformGradient_().clone().ptr()
    : NULL
  }
{
  // For safety re-evaluate
  const scalar t = this->db().time().timeOutputValue();
  if (ptf.uniformGradient_.valid()) {
    this->gradient() = uniformGradient_->value(t);
  }
}


// Member Functions 
template<class Type>
void mousse::uniformFixedGradientFvPatchField<Type>::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  const scalar t = this->db().time().timeOutputValue();
  this->gradient() = uniformGradient_->value(t);
  fixedGradientFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void mousse::uniformFixedGradientFvPatchField<Type>::write(Ostream& os) const
{
  fixedGradientFvPatchField<Type>::write(os);
  uniformGradient_->writeData(os);
  this->writeEntry("value", os);
}

