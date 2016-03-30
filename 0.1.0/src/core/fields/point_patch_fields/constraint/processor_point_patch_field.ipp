// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_point_patch_field.hpp"
#include "processor_poly_patch.hpp"


namespace mousse {

// Constructors
template<class Type>
processorPointPatchField<Type>::processorPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  coupledPointPatchField<Type>{p, iF},
  procPatch_{refCast<const processorPointPatch>(p)}
{}


template<class Type>
processorPointPatchField<Type>::processorPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  coupledPointPatchField<Type>{p, iF, dict},
  procPatch_{refCast<const processorPointPatch>(p)}
{}


template<class Type>
processorPointPatchField<Type>::processorPointPatchField
(
  const processorPointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  coupledPointPatchField<Type>{ptf, p, iF, mapper},
  procPatch_{refCast<const processorPointPatch>(ptf.patch())}
{}


template<class Type>
processorPointPatchField<Type>::processorPointPatchField
(
  const processorPointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  coupledPointPatchField<Type>{ptf, iF},
  procPatch_{refCast<const processorPointPatch>(ptf.patch())}
{}


// Destructor 
template<class Type>
processorPointPatchField<Type>::~processorPointPatchField()
{}

}  // namespace mousse
