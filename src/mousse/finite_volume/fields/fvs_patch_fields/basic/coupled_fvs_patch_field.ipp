// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coupled_fvs_patch_field.hpp"


namespace mousse {

// Member Functions 
template<class Type>
coupledFvsPatchField<Type>::coupledFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  fvsPatchField<Type>{p, iF}
{}


template<class Type>
coupledFvsPatchField<Type>::coupledFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const Field<Type>& f
)
:
  fvsPatchField<Type>{p, iF, f}
{}


template<class Type>
coupledFvsPatchField<Type>::coupledFvsPatchField
(
  const coupledFvsPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fvsPatchField<Type>{ptf, p, iF, mapper}
{}


template<class Type>
coupledFvsPatchField<Type>::coupledFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const dictionary& dict
)
:
  fvsPatchField<Type>{p, iF, dict}
{}


template<class Type>
coupledFvsPatchField<Type>::coupledFvsPatchField
(
  const coupledFvsPatchField<Type>& ptf
)
:
  fvsPatchField<Type>{ptf}
{}


template<class Type>
coupledFvsPatchField<Type>::coupledFvsPatchField
(
  const coupledFvsPatchField<Type>& ptf,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  fvsPatchField<Type>{ptf, iF}
{}

}  // namespace mousse

