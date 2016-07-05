// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_fixed_pushed_internal_value_fv_patch_field.hpp"
#include "uindirect_list.hpp"


// Constructors 
template<class Type>
mousse::mappedFixedPushedInternalValueFvPatchField<Type>::
mappedFixedPushedInternalValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  mappedFixedValueFvPatchField<Type>{p, iF}
{}


template<class Type>
mousse::mappedFixedPushedInternalValueFvPatchField<Type>::
mappedFixedPushedInternalValueFvPatchField
(
  const mappedFixedPushedInternalValueFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mappedFixedValueFvPatchField<Type>{ptf, p, iF, mapper}
{}


template<class Type>
mousse::mappedFixedPushedInternalValueFvPatchField<Type>::
mappedFixedPushedInternalValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  mappedFixedValueFvPatchField<Type>{p, iF, dict}
{}


template<class Type>
mousse::mappedFixedPushedInternalValueFvPatchField<Type>::
mappedFixedPushedInternalValueFvPatchField
(
  const mappedFixedPushedInternalValueFvPatchField<Type>& ptf
)
:
  mappedFixedValueFvPatchField<Type>{ptf}
{}


template<class Type>
mousse::mappedFixedPushedInternalValueFvPatchField<Type>::
mappedFixedPushedInternalValueFvPatchField
(
  const mappedFixedPushedInternalValueFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  mappedFixedValueFvPatchField<Type>{ptf, iF}
{}


// Member Functions 
template<class Type>
void mousse::mappedFixedPushedInternalValueFvPatchField<Type>::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  // Retrieve the neighbour values and assign to this patch boundary field
  mappedFixedValueFvPatchField<Type>::updateCoeffs();
  // Assign the patch internal field to its boundary value
  Field<Type>& intFld = const_cast<Field<Type>&>(this->internalField());
  UIndirectList<Type>(intFld, this->patch().faceCells()) = *this;
}


template<class Type>
void mousse::mappedFixedPushedInternalValueFvPatchField<Type>::write
(
  Ostream& os
) const
{
  mappedFixedValueFvPatchField<Type>::write(os);
}

