// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nonuniform_transform_cyclic_fv_patch_field.hpp"


namespace mousse {

// Constructors 
template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  cyclicFvPatchField<Type>{p, iF}
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
  const nonuniformTransformCyclicFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  cyclicFvPatchField<Type>{ptf, p, iF, mapper}
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  cyclicFvPatchField<Type>{p, iF, dict}
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
  const nonuniformTransformCyclicFvPatchField<Type>& ptf
)
:
  cyclicFvPatchField<Type>{ptf}
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
  const nonuniformTransformCyclicFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  cyclicFvPatchField<Type>{ptf, iF}
{}

}  // namespace mousse

