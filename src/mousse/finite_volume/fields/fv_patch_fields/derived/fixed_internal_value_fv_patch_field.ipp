// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_internal_value_fv_patch_field.hpp"
#include "fv_patch_field_mapper.hpp"
#include "fv_matrix.hpp"


// Constructors 
template<class Type>
mousse::fixedInternalValueFvPatchField<Type>::fixedInternalValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  zeroGradientFvPatchField<Type>{p, iF}
{}


template<class Type>
mousse::fixedInternalValueFvPatchField<Type>::fixedInternalValueFvPatchField
(
  const fixedInternalValueFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  zeroGradientFvPatchField<Type>{ptf, p, iF, mapper}
{}


template<class Type>
mousse::fixedInternalValueFvPatchField<Type>::fixedInternalValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  zeroGradientFvPatchField<Type>{p, iF, dict}
{}


template<class Type>
mousse::fixedInternalValueFvPatchField<Type>::fixedInternalValueFvPatchField
(
  const fixedInternalValueFvPatchField& fivpf
)
:
  zeroGradientFvPatchField<Type>{fivpf}
{}


template<class Type>
mousse::fixedInternalValueFvPatchField<Type>::fixedInternalValueFvPatchField
(
  const fixedInternalValueFvPatchField& fivpf,
  const DimensionedField<Type, volMesh>& iF
)
:
  zeroGradientFvPatchField<Type>{fivpf, iF}
{}


// Member Functions 
template<class Type>
void mousse::fixedInternalValueFvPatchField<Type>::manipulateMatrix
(
  fvMatrix<Type>& matrix
)
{
  // Apply the patch internal field as a constraint in the matrix
  matrix.setValues(this->patch().faceCells(), this->patchInternalField());
}

