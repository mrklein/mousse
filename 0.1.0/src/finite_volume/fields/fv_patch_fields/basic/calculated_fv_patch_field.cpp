// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calculated_fv_patch_field.hpp"
#include "fv_patch_field_mapper.hpp"
template<class Type>
const mousse::word& mousse::fvPatchField<Type>::calculatedType()
{
  return calculatedFvPatchField<Type>::typeName;
}
// Constructors 
template<class Type>
mousse::calculatedFvPatchField<Type>::calculatedFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fvPatchField<Type>(p, iF)
{}
template<class Type>
mousse::calculatedFvPatchField<Type>::calculatedFvPatchField
(
  const calculatedFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fvPatchField<Type>(ptf, p, iF, mapper)
{}
template<class Type>
mousse::calculatedFvPatchField<Type>::calculatedFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict,
  const bool valueRequired
)
:
  fvPatchField<Type>(p, iF, dict, valueRequired)
{}
template<class Type>
mousse::calculatedFvPatchField<Type>::calculatedFvPatchField
(
  const calculatedFvPatchField<Type>& ptf
)
:
  fvPatchField<Type>(ptf)
{}
template<class Type>
mousse::calculatedFvPatchField<Type>::calculatedFvPatchField
(
  const calculatedFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fvPatchField<Type>(ptf, iF)
{}
template<class Type>
mousse::tmp<mousse::fvPatchField<Type> >
mousse::fvPatchField<Type>::NewCalculatedType
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
      DimensionedField<Type, volMesh>::null()
    );
  }
  else
  {
    return tmp<fvPatchField<Type> >
    (
      new calculatedFvPatchField<Type>
      (
        p,
        DimensionedField<Type, volMesh>::null()
      )
    );
  }
}
template<class Type>
template<class Type2>
mousse::tmp<mousse::fvPatchField<Type> > mousse::fvPatchField<Type>::NewCalculatedType
(
  const fvPatchField<Type2>& pf
)
{
  return NewCalculatedType(pf.patch());
}
// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::calculatedFvPatchField<Type>::valueInternalCoeffs
(
  const tmp<scalarField>&
) const
{
  FatalErrorIn
  (
    "calculatedFvPatchField<Type>::"
    "valueInternalCoeffs(const tmp<scalarField>&) const"
  )   << "\n    "
     "valueInternalCoeffs cannot be called for a calculatedFvPatchField"
    << "\n    on patch " << this->patch().name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << "\n    You are probably trying to solve for a field with a "
     "default boundary condition."
    << exit(FatalError);
  return *this;
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::calculatedFvPatchField<Type>::valueBoundaryCoeffs
(
  const tmp<scalarField>&
) const
{
  FatalErrorIn
  (
    "calculatedFvPatchField<Type>::"
    "valueBoundaryCoeffs(const tmp<scalarField>&) const"
  )   << "\n    "
     "valueBoundaryCoeffs cannot be called for a calculatedFvPatchField"
    << "\n    on patch " << this->patch().name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << "\n    You are probably trying to solve for a field with a "
     "default boundary condition."
    << exit(FatalError);
  return *this;
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::calculatedFvPatchField<Type>::gradientInternalCoeffs() const
{
  FatalErrorIn
  (
    "calculatedFvPatchField<Type>::"
    "gradientInternalCoeffs() const"
  )   << "\n    "
     "gradientInternalCoeffs cannot be called for a "
     "calculatedFvPatchField"
    << "\n    on patch " << this->patch().name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << "\n    You are probably trying to solve for a field with a "
     "default boundary condition."
    << exit(FatalError);
  return *this;
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::calculatedFvPatchField<Type>::gradientBoundaryCoeffs() const
{
  FatalErrorIn
  (
    "calculatedFvPatchField<Type>::"
    "gradientBoundaryCoeffs() const"
  )   << "\n    "
     "gradientBoundaryCoeffs cannot be called for a "
     "calculatedFvPatchField"
    << "\n    on patch " << this->patch().name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << "\n    You are probably trying to solve for a field with a "
     "default boundary condition."
    << exit(FatalError);
  return *this;
}
template<class Type>
void mousse::calculatedFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  this->writeEntry("value", os);
}
