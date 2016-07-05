// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetry_point_patch_field.hpp"


namespace mousse {

// Constructors 
template<class Type>
symmetryPointPatchField<Type>::symmetryPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  basicSymmetryPointPatchField<Type>{p, iF}
{}


template<class Type>
symmetryPointPatchField<Type>::symmetryPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  basicSymmetryPointPatchField<Type>{p, iF, dict}
{
  if (!isType<symmetryPointPatch>(p)) {
    FATAL_IO_ERROR_IN
    (
      "symmetryPointPatchField<Type>::symmetryPointPatchField\n"
      "(\n"
      "    const pointPatch& p,\n"
      "    const Field<Type>& field,\n"
      "    const dictionary& dict\n"
      ")\n",
      dict
    )
    << "patch " << this->patch().index() << " not symmetry type. "
    << "Patch type = " << p.type()
    << exit(FatalIOError);
  }
}


template<class Type>
symmetryPointPatchField<Type>::symmetryPointPatchField
(
  const symmetryPointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  basicSymmetryPointPatchField<Type>{ptf, p, iF, mapper}
{
  if (!isType<symmetryPointPatch>(this->patch())) {
    FATAL_ERROR_IN
    (
      "symmetryPointPatchField<Type>::symmetryPointPatchField\n"
      "(\n"
      "    const symmetryPointPatchField<Type>& ptf,\n"
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
symmetryPointPatchField<Type>::symmetryPointPatchField
(
  const symmetryPointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  basicSymmetryPointPatchField<Type>{ptf, iF}
{}

}  // namespace mousse
