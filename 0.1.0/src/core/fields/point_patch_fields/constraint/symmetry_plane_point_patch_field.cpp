// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetry_plane_point_patch_field.hpp"

namespace mousse
{
// Constructors 
template<class Type>
symmetryPlanePointPatchField<Type>::symmetryPlanePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  basicSymmetryPointPatchField<Type>{p, iF},
  symmetryPlanePatch_{refCast<const symmetryPlanePointPatch>(p)}
{}


template<class Type>
symmetryPlanePointPatchField<Type>::symmetryPlanePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  basicSymmetryPointPatchField<Type>{p, iF, dict},
  symmetryPlanePatch_{refCast<const symmetryPlanePointPatch>(p)}
{
  if (!isType<symmetryPlanePointPatch>(p))
  {
    FATAL_IO_ERROR_IN
    (
      "symmetryPlanePointPatchField<Type>::symmetryPlanePointPatchField\n"
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
symmetryPlanePointPatchField<Type>::symmetryPlanePointPatchField
(
  const symmetryPlanePointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  basicSymmetryPointPatchField<Type>{ptf, p, iF, mapper},
  symmetryPlanePatch_{refCast<const symmetryPlanePointPatch>(p)}
{
  if (!isType<symmetryPlanePointPatch>(this->patch()))
  {
    FATAL_ERROR_IN
    (
      "symmetryPlanePointPatchField<Type>::symmetryPlanePointPatchField\n"
      "(\n"
      "    const symmetryPlanePointPatchField<Type>& ptf,\n"
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
symmetryPlanePointPatchField<Type>::symmetryPlanePointPatchField
(
  const symmetryPlanePointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  basicSymmetryPointPatchField<Type>{ptf, iF},
  symmetryPlanePatch_{ptf.symmetryPlanePatch_}
{}


// Member Functions 
template<class Type>
void mousse::symmetryPlanePointPatchField<Type>::evaluate
(
  const Pstream::commsTypes
)
{
  vector nHat = symmetryPlanePatch_.n();
  tmp<Field<Type> > tvalues =
  (
    (
      this->patchInternalField()
     + transform(I - 2.0*sqr(nHat), this->patchInternalField())
    )/2.0
  );
  // Get internal field to insert values into
  Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());
  this->setInInternalField(iF, tvalues());
}

}  // namespace mousse
