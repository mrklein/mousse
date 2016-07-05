// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nonuniform_transform_cyclic_fvs_patch_field.hpp"


namespace mousse {

// Constructors 
template<class Type>
nonuniformTransformCyclicFvsPatchField<Type>::
nonuniformTransformCyclicFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  cyclicFvsPatchField<Type>{p, iF}
{}


template<class Type>
nonuniformTransformCyclicFvsPatchField<Type>::
nonuniformTransformCyclicFvsPatchField
(
  const nonuniformTransformCyclicFvsPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  cyclicFvsPatchField<Type>{ptf, p, iF, mapper}
{}


template<class Type>
nonuniformTransformCyclicFvsPatchField<Type>::
nonuniformTransformCyclicFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const dictionary& dict
)
:
  cyclicFvsPatchField<Type>{p, iF, dict}
{}


template<class Type>
nonuniformTransformCyclicFvsPatchField<Type>::
nonuniformTransformCyclicFvsPatchField
(
  const nonuniformTransformCyclicFvsPatchField<Type>& ptf
)
:
  cyclicFvsPatchField<Type>{ptf}
{}


template<class Type>
nonuniformTransformCyclicFvsPatchField<Type>::
nonuniformTransformCyclicFvsPatchField
(
  const nonuniformTransformCyclicFvsPatchField<Type>& ptf,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  cyclicFvsPatchField<Type>{ptf, iF}
{}

}  // namespace mousse
