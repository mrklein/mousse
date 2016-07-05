// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "slip_point_patch_field.hpp"


namespace mousse {

// Constructors 
template<class Type>
slipPointPatchField<Type>::slipPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  basicSymmetryPointPatchField<Type>{p, iF}
{}


template<class Type>
slipPointPatchField<Type>::slipPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  basicSymmetryPointPatchField<Type>{p, iF, dict}
{}


template<class Type>
slipPointPatchField<Type>::slipPointPatchField
(
  const slipPointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  basicSymmetryPointPatchField<Type>{ptf, p, iF, mapper}
{}


template<class Type>
slipPointPatchField<Type>::slipPointPatchField
(
  const slipPointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  basicSymmetryPointPatchField<Type>{ptf, iF}
{}

}  // namespace mousse

