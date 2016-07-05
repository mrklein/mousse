// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "value_point_patch_field.hpp"
#include "point_patch_field_mapper.hpp"


// Private Member Functions 
template<class Type>
void mousse::valuePointPatchField<Type>::checkFieldSize() const
{
  if (this->size() != this->patch().size()) {
    FATAL_ERROR_IN
    (
      "void valuePointPatchField<Type>::checkField() const"
    )
    << "field does not correspond to patch. " << endl
    << "Field size: " << size() << " patch size: "
    << this->patch().size()
    << abort(FatalError);
  }
}


// Constructors
template<class Type>
mousse::valuePointPatchField<Type>::valuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  pointPatchField<Type>{p, iF},
  Field<Type>{p.size()}
{}


template<class Type>
mousse::valuePointPatchField<Type>::valuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict,
  const bool valueRequired
)
:
  pointPatchField<Type>{p, iF, dict},
  Field<Type>{p.size()}
{
  if (dict.found("value")) {
    Field<Type>::operator=
    (
      Field<Type>("value", dict, p.size())
    );
  } else if (!valueRequired) {
    Field<Type>::operator=(pTraits<Type>::zero);
  } else {
    FATAL_IO_ERROR_IN
    (
      "pointPatchField<Type>::pointPatchField"
      "("
      "const fvPatch& p,"
      "const DimensionedField<Type, pointMesh>& iF,"
      "const dictionary& dict,"
      "const bool valueRequired"
      ")",
      dict
    )
    << "Essential entry 'value' missing"
    << exit(FatalIOError);
  }
}


template<class Type>
mousse::valuePointPatchField<Type>::valuePointPatchField
(
  const valuePointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  pointPatchField<Type>{ptf, p, iF, mapper},
  Field<Type>{ptf, mapper}
{}


template<class Type>
mousse::valuePointPatchField<Type>::valuePointPatchField
(
  const valuePointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  pointPatchField<Type>{ptf, iF},
  Field<Type>{ptf}
{}


// Member Functions 
template<class Type>
void mousse::valuePointPatchField<Type>::autoMap
(
  const pointPatchFieldMapper& m
)
{
  Field<Type>::autoMap(m);
}


template<class Type>
void mousse::valuePointPatchField<Type>::rmap
(
  const pointPatchField<Type>& ptf,
  const labelList& addr
)
{
  Field<Type>::rmap
  (
    refCast<const valuePointPatchField<Type>>
    (
      ptf
    ),
    addr
  );
}


template<class Type>
void mousse::valuePointPatchField<Type>::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  // Get internal field to insert values into
  Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());
  this->setInInternalField(iF, *this);
  pointPatchField<Type>::updateCoeffs();
}


template<class Type>
void mousse::valuePointPatchField<Type>::evaluate(const Pstream::commsTypes)
{
  // Get internal field to insert values into
  Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());
  this->setInInternalField(iF, *this);
  pointPatchField<Type>::evaluate();
}


template<class Type>
void mousse::valuePointPatchField<Type>::write(Ostream& os) const
{
  pointPatchField<Type>::write(os);
  this->writeEntry("value", os);
}


// Member Operators 
template<class Type>
void mousse::valuePointPatchField<Type>::operator=
(
  const valuePointPatchField<Type>& ptf
)
{
  Field<Type>::operator=(ptf);
}


template<class Type>
void mousse::valuePointPatchField<Type>::operator=
(
  const pointPatchField<Type>& ptf
)
{
  Field<Type>::operator=(ptf.patchInternalField());
}


template<class Type>
void mousse::valuePointPatchField<Type>::operator=
(
  const Field<Type>& tf
)
{
  Field<Type>::operator=(tf);
}


template<class Type>
void mousse::valuePointPatchField<Type>::operator=
(
  const Type& t
)
{
  Field<Type>::operator=(t);
}


// Force an assignment
template<class Type>
void mousse::valuePointPatchField<Type>::operator==
(
  const valuePointPatchField<Type>& ptf
)
{
  Field<Type>::operator=(ptf);
}


template<class Type>
void mousse::valuePointPatchField<Type>::operator==
(
  const pointPatchField<Type>& ptf
)
{
  Field<Type>::operator=(ptf.patchInternalField());
}


template<class Type>
void mousse::valuePointPatchField<Type>::operator==
(
  const Field<Type>& tf
)
{
  Field<Type>::operator=(tf);
}


template<class Type>
void mousse::valuePointPatchField<Type>::operator==
(
  const Type& t
)
{
  Field<Type>::operator=(t);
}
