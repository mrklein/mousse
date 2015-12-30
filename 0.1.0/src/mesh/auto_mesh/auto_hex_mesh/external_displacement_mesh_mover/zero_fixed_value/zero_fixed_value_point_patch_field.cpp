// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "zero_fixed_value_point_patch_field.hpp"
namespace mousse
{
// Constructors
template<class Type>
zeroFixedValuePointPatchField<Type>::
zeroFixedValuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  fixedValuePointPatchField<Type>(p, iF)
{}
template<class Type>
zeroFixedValuePointPatchField<Type>::
zeroFixedValuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchField<Type>(p, iF, dict, false)
{
  fixedValuePointPatchField<Type>::operator=(pTraits<Type>::zero);
}
template<class Type>
zeroFixedValuePointPatchField<Type>::
zeroFixedValuePointPatchField
(
  const zeroFixedValuePointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<Type>(ptf, p, iF, mapper)
{
  // For safety re-evaluate
  fixedValuePointPatchField<Type>::operator=(pTraits<Type>::zero);
}
template<class Type>
zeroFixedValuePointPatchField<Type>::
zeroFixedValuePointPatchField
(
  const zeroFixedValuePointPatchField<Type>& ptf
)
:
  fixedValuePointPatchField<Type>(ptf)
{}
template<class Type>
zeroFixedValuePointPatchField<Type>::
zeroFixedValuePointPatchField
(
  const zeroFixedValuePointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  fixedValuePointPatchField<Type>(ptf, iF)
{
  // For safety re-evaluate
  fixedValuePointPatchField<Type>::operator=(pTraits<Type>::zero);
}
}  // namespace mousse
