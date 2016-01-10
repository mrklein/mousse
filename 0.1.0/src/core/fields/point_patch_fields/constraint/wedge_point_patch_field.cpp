// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wedge_point_patch_field.hpp"
#include "transform_field.hpp"

// Constructors 
template<class Type>
mousse::wedgePointPatchField<Type>::wedgePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  pointPatchField<Type>{p, iF}
{}

template<class Type>
mousse::wedgePointPatchField<Type>::wedgePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  pointPatchField<Type>{p, iF, dict}
{
  if (!isType<wedgePointPatch>(p))
  {
    FATAL_IO_ERROR_IN
    (
      "wedgePointPatchField<Type>::wedgePointPatchField\n"
      "(\n"
      "    const pointPatch& p,\n"
      "    const Field<Type>& field,\n"
      "    const dictionary& dict\n"
      ")\n",
      dict
    )
    << "patch " << this->patch().index() << " not wedge type. "
    << "Patch type = " << p.type()
    << exit(FatalIOError);
  }
}


template<class Type>
mousse::wedgePointPatchField<Type>::wedgePointPatchField
(
  const wedgePointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  pointPatchField<Type>{ptf, p, iF, mapper}
{
  if (!isType<wedgePointPatch>(this->patch()))
  {
    FATAL_ERROR_IN
    (
      "wedgePointPatchField<Type>::wedgePointPatchField\n"
      "(\n"
      "    const wedgePointPatchField<Type>& ptf,\n"
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
mousse::wedgePointPatchField<Type>::wedgePointPatchField
(
  const wedgePointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  pointPatchField<Type>{ptf, iF}
{}


// Member Functions 
template<class Type>
void mousse::wedgePointPatchField<Type>::evaluate(const Pstream::commsTypes)
{
  // In order to ensure that the wedge patch is always flat, take the
  // normal vector from the first point
  const vector& nHat = this->patch().pointNormals()[0];
  tmp<Field<Type> > tvalues =
    transform(I - nHat*nHat, this->patchInternalField());
  // Get internal field to insert values into
  Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());
  this->setInInternalField(iF, tvalues());
}
