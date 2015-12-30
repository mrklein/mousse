// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calculated_fvs_patch_field.hpp"
#include "fv_patch_field_mapper.hpp"
namespace mousse
{
template<class Type>
const word& fvsPatchField<Type>::calculatedType()
{
  return calculatedFvsPatchField<Type>::typeName;
}
// Constructors 
template<class Type>
calculatedFvsPatchField<Type>::calculatedFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  fvsPatchField<Type>(p, iF)
{}
template<class Type>
calculatedFvsPatchField<Type>::calculatedFvsPatchField
(
  const calculatedFvsPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fvsPatchField<Type>(ptf, p, iF, mapper)
{}
template<class Type>
calculatedFvsPatchField<Type>::calculatedFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const dictionary& dict
)
:
  fvsPatchField<Type>(p, iF, Field<Type>("value", dict, p.size()))
{}
template<class Type>
calculatedFvsPatchField<Type>::calculatedFvsPatchField
(
  const calculatedFvsPatchField<Type>& ptf
)
:
  fvsPatchField<Type>(ptf)
{}
template<class Type>
calculatedFvsPatchField<Type>::calculatedFvsPatchField
(
  const calculatedFvsPatchField<Type>& ptf,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  fvsPatchField<Type>(ptf, iF)
{}
template<class Type>
tmp<fvsPatchField<Type> > fvsPatchField<Type>::NewCalculatedType
(
  const fvPatch& p
)
{
  typename patchConstructorTable::iterator patchTypeCstrIter =
    patchConstructorTablePtr_->find(p.type());
  if (patchTypeCstrIter != patchConstructorTablePtr_->end())
  {
    return patchTypeCstrIter()
    (
      p,
      DimensionedField<Type, surfaceMesh>::null()
    );
  }
  else
  {
    return tmp<fvsPatchField<Type> >
    (
      new calculatedFvsPatchField<Type>
      (
        p,
        DimensionedField<Type, surfaceMesh>::null()
      )
    );
  }
}
template<class Type>
template<class Type2>
tmp<fvsPatchField<Type> > fvsPatchField<Type>::NewCalculatedType
(
  const fvsPatchField<Type2>& pf
)
{
  return NewCalculatedType(pf.patch());
}
}  // namespace mousse
