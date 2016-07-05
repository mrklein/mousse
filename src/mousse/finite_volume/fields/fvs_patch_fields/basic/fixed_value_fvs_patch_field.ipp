// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_value_fvs_patch_field.hpp"


namespace mousse {

// Member Functions 
template<class Type>
fixedValueFvsPatchField<Type>::fixedValueFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  fvsPatchField<Type>{p, iF}
{}


template<class Type>
fixedValueFvsPatchField<Type>::fixedValueFvsPatchField
(
  const fixedValueFvsPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fvsPatchField<Type>{ptf, p, iF, mapper}
{}


template<class Type>
fixedValueFvsPatchField<Type>::fixedValueFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const dictionary& dict
)
:
  fvsPatchField<Type>{p, iF, Field<Type>{"value", dict, p.size()}}
{}


template<class Type>
fixedValueFvsPatchField<Type>::fixedValueFvsPatchField
(
  const fixedValueFvsPatchField<Type>& ptf
)
:
  fvsPatchField<Type>{ptf}
{}


template<class Type>
fixedValueFvsPatchField<Type>::fixedValueFvsPatchField
(
  const fixedValueFvsPatchField<Type>& ptf,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  fvsPatchField<Type>{ptf, iF}
{}


// Member Functions 
template<class Type>
tmp<Field<Type> > fixedValueFvsPatchField<Type>::valueInternalCoeffs
(
  const tmp<scalarField>&
) const
{
  return tmp<Field<Type>>
  {
    new Field<Type>{this->size(), pTraits<Type>::zero}
  };
}


template<class Type>
tmp<Field<Type> > fixedValueFvsPatchField<Type>::valueBoundaryCoeffs
(
  const tmp<scalarField>&
) const
{
  return *this;
}


template<class Type>
tmp<Field<Type> > fixedValueFvsPatchField<Type>::gradientInternalCoeffs() const
{
  return -pTraits<Type>::one*this->patch().deltaCoeffs();
}


template<class Type>
tmp<Field<Type> > fixedValueFvsPatchField<Type>::gradientBoundaryCoeffs() const
{
  return this->patch().deltaCoeffs()*(*this);
}

}  // namespace mousse
