// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_value_point_patch_field.hpp"
#include "bool_list.hpp"
namespace mousse
{
// Constructors 
template<class Type>
fixedValuePointPatchField<Type>::fixedValuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  valuePointPatchField<Type>(p, iF)
{}
template<class Type>
fixedValuePointPatchField<Type>::fixedValuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict,
  const bool valueRequired
)
:
  valuePointPatchField<Type>(p, iF, dict, valueRequired)
{}
template<class Type>
fixedValuePointPatchField<Type>::fixedValuePointPatchField
(
  const fixedValuePointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  valuePointPatchField<Type>(ptf, p, iF, mapper)
{}
template<class Type>
fixedValuePointPatchField<Type>::fixedValuePointPatchField
(
  const fixedValuePointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  valuePointPatchField<Type>(ptf, iF)
{}
}  // namespace mousse
