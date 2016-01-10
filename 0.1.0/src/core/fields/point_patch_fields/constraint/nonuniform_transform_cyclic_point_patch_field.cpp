// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nonuniform_transform_cyclic_point_patch_field.hpp"
#include "transform_field.hpp"
#include "symm_transform_field.hpp"
// Constructors 
template<class Type>
mousse::nonuniformTransformCyclicPointPatchField<Type>::
nonuniformTransformCyclicPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  cyclicPointPatchField<Type>(p, iF)
{}
template<class Type>
mousse::nonuniformTransformCyclicPointPatchField<Type>::
nonuniformTransformCyclicPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  cyclicPointPatchField<Type>(p, iF, dict)
{}
template<class Type>
mousse::nonuniformTransformCyclicPointPatchField<Type>::
nonuniformTransformCyclicPointPatchField
(
  const nonuniformTransformCyclicPointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  cyclicPointPatchField<Type>(ptf, p, iF, mapper)
{}
template<class Type>
mousse::nonuniformTransformCyclicPointPatchField<Type>::
nonuniformTransformCyclicPointPatchField
(
  const nonuniformTransformCyclicPointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  cyclicPointPatchField<Type>(ptf, iF)
{}
// Member Functions 
template<class Type>
void mousse::nonuniformTransformCyclicPointPatchField<Type>::evaluate
(
  const Pstream::commsTypes
)
{
  const vectorField& nHat = this->patch().pointNormals();
  tmp<Field<Type> > tvalues =
  (
    (
      this->patchInternalField()
     + transform(I - 2.0*sqr(nHat), this->patchInternalField())
    )/2.0
  );
  // Get internal field to insert values into
  Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());
  this->setInInternalField(iF, tvalues());
}
