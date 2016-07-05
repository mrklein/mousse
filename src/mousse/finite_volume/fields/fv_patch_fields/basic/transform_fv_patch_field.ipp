// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "transform_fv_patch_field.hpp"
#include "iostreams.hpp"
#include "transform_field.hpp"


// Member Functions 
template<class Type>
mousse::transformFvPatchField<Type>::transformFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fvPatchField<Type>{p, iF}
{}


template<class Type>
mousse::transformFvPatchField<Type>::transformFvPatchField
(
  const transformFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fvPatchField<Type>{ptf, p, iF, mapper}
{}


template<class Type>
mousse::transformFvPatchField<Type>::transformFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fvPatchField<Type>{p, iF, dict}
{}


template<class Type>
mousse::transformFvPatchField<Type>::transformFvPatchField
(
  const transformFvPatchField<Type>& ptf
)
:
  fvPatchField<Type>{ptf}
{}


template<class Type>
mousse::transformFvPatchField<Type>::transformFvPatchField
(
  const transformFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fvPatchField<Type>{ptf, iF}
{}


// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::transformFvPatchField<Type>::valueInternalCoeffs
(
  const tmp<scalarField>&
) const
{
  return pTraits<Type>::one - snGradTransformDiag();
}


template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::transformFvPatchField<Type>::valueBoundaryCoeffs
(
  const tmp<scalarField>&
) const
{
  return
    *this - cmptMultiply(valueInternalCoeffs(this->patch().weights()),
                         this->patchInternalField());
}


template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::transformFvPatchField<Type>::gradientInternalCoeffs() const
{
  return -this->patch().deltaCoeffs()*snGradTransformDiag();
}


template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::transformFvPatchField<Type>::gradientBoundaryCoeffs() const
{
  return
    snGrad() - cmptMultiply(gradientInternalCoeffs(),
                            this->patchInternalField());
}


// Member Operators 
template<class Type>
void mousse::transformFvPatchField<Type>::operator=
(
  const fvPatchField<Type>& /*ptf*/
)
{
  this->evaluate();
}
