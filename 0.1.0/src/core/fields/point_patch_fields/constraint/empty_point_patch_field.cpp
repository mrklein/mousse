// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "empty_point_patch_field.hpp"

namespace mousse
{

// Constructors 
template<class Type>
emptyPointPatchField<Type>::emptyPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  pointPatchField<Type>{p, iF}
{}


template<class Type>
emptyPointPatchField<Type>::emptyPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  pointPatchField<Type>{p, iF, dict}
{
  if (!isType<emptyPointPatch>(p))
  {
    FATAL_IO_ERROR_IN
    (
      "emptyPointPatchField<Type>::emptyPointPatchField\n"
      "(\n"
      "    const pointPatch& p,\n"
      "    const Field<Type>& field,\n"
      "    const dictionary& dict\n"
      ")\n",
      dict
    )
    << "patch " << this->patch().index() << " not empty type. "
    << "Patch type = " << p.type()
    << exit(FatalIOError);
  }
}


template<class Type>
emptyPointPatchField<Type>::emptyPointPatchField
(
  const emptyPointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  pointPatchField<Type>{ptf, p, iF, mapper}
{
  if (!isType<emptyPointPatch>(this->patch()))
  {
    FATAL_ERROR_IN
    (
      "emptyPointPatchField<Type>::emptyPointPatchField\n"
      "(\n"
      "    const emptyPointPatchField<Type>& ptf,\n"
      "    const pointPatch& p,\n"
      "    const DimensionedField<Type, pointMesh>& iF,\n"
      "    const pointPatchFieldMapper& mapper\n"
      ")\n"
    )
    << "Field type does not correspond to patch type for patch "
    << this->patch().index() << "." << endl
    << "Field type: " << typeName << endl
    << "Patch type: " << this->patch().type()
    << exit(FatalError);
  }
}


template<class Type>
emptyPointPatchField<Type>::emptyPointPatchField
(
  const emptyPointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  pointPatchField<Type>{ptf, iF}
{}

}  // namespace mousse
