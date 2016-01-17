// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coupled_point_patch_field.hpp"
namespace mousse
{
// Constructors
template<class Type>
coupledPointPatchField<Type>::coupledPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  pointPatchField<Type>(p, iF)
{}
template<class Type>
coupledPointPatchField<Type>::coupledPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  pointPatchField<Type>(p, iF, dict)
{}
template<class Type>
coupledPointPatchField<Type>::coupledPointPatchField
(
  const coupledPointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  pointPatchField<Type>(ptf, p, iF, mapper)
{}
template<class Type>
coupledPointPatchField<Type>::coupledPointPatchField
(
  const coupledPointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  pointPatchField<Type>(ptf, iF)
{}
}  // namespace mousse
