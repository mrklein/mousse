// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_value_fv_patch_field.hpp"

namespace mousse
{

// Member Functions 
template<class Type>
fixedValueFvPatchField<Type>::fixedValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fvPatchField<Type>{p, iF}
{}


template<class Type>
fixedValueFvPatchField<Type>::fixedValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fvPatchField<Type>{p, iF, dict, true}
{}


template<class Type>
fixedValueFvPatchField<Type>::fixedValueFvPatchField
(
  const fixedValueFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fvPatchField<Type>{ptf, p, iF, mapper}
{
  if (notNull(iF) && mapper.hasUnmapped())
  {
    WARNING_IN
    (
      "fixedValueFvPatchField<Type>::fixedValueFvPatchField\n"
      "(\n"
      "    const fixedValueFvPatchField<Type>&,\n"
      "    const fvPatch&,\n"
      "    const DimensionedField<Type, volMesh>&,\n"
      "    const fvPatchFieldMapper&\n"
      ")\n"
    )
    << "On field " << iF.name() << " patch " << p.name()
    << " patchField " << this->type()
    << " : mapper does not map all values." << nl
    << "    To avoid this warning fully specify the mapping in derived"
    << " patch fields." << endl;
  }
}


template<class Type>
fixedValueFvPatchField<Type>::fixedValueFvPatchField
(
  const fixedValueFvPatchField<Type>& ptf
)
:
  fvPatchField<Type>{ptf}
{}


template<class Type>
fixedValueFvPatchField<Type>::fixedValueFvPatchField
(
  const fixedValueFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fvPatchField<Type>{ptf, iF}
{}


// Member Functions 

template<class Type>
tmp<Field<Type> > fixedValueFvPatchField<Type>::valueInternalCoeffs
(
  const tmp<scalarField>&
) const
{
  return tmp<Field<Type> >
  {
    new Field<Type>{this->size(), pTraits<Type>::zero}
  };
}


template<class Type>
tmp<Field<Type> > fixedValueFvPatchField<Type>::valueBoundaryCoeffs
(
  const tmp<scalarField>&
) const
{
  return *this;
}


template<class Type>
tmp<Field<Type> > fixedValueFvPatchField<Type>::gradientInternalCoeffs() const
{
  return -pTraits<Type>::one*this->patch().deltaCoeffs();
}


template<class Type>
tmp<Field<Type> > fixedValueFvPatchField<Type>::gradientBoundaryCoeffs() const
{
  return this->patch().deltaCoeffs()*(*this);
}


template<class Type>
void fixedValueFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  this->writeEntry("value", os);
}

}  // namespace mousse
