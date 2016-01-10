// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetry_plane_fvs_patch_field.hpp"

namespace mousse
{

// Constructors 
template<class Type>
symmetryPlaneFvsPatchField<Type>::symmetryPlaneFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  fvsPatchField<Type>{p, iF}
{}

template<class Type>
symmetryPlaneFvsPatchField<Type>::symmetryPlaneFvsPatchField
(
  const symmetryPlaneFvsPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fvsPatchField<Type>{ptf, p, iF, mapper}
{
  if (!isType<symmetryPlaneFvPatch>(this->patch()))
  {
    FATAL_ERROR_IN
    (
      "symmetryPlaneFvsPatchField<Type>::symmetryPlaneFvsPatchField\n"
      "(\n"
      "    const symmetryPlaneFvsPatchField<Type>& ptf,\n"
      "    const fvPatch& p,\n"
      "    const DimensionedField<Type, surfaceMesh>& iF,\n"
      "    const fvPatchFieldMapper& mapper\n"
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
symmetryPlaneFvsPatchField<Type>::symmetryPlaneFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const dictionary& dict
)
:
  fvsPatchField<Type>{p, iF, dict}
{
  if (!isType<symmetryPlaneFvPatch>(p))
  {
    FATAL_IO_ERROR_IN
    (
      "symmetryPlaneFvsPatchField<Type>::symmetryPlaneFvsPatchField\n"
      "(\n"
      "    const fvPatch& p,\n"
      "    const Field<Type>& field,\n"
      "    const dictionary& dict\n"
      ")\n",
      dict
    )
    << "patch " << this->patch().index() << " not symmetryPlane type. "
    << "Patch type = " << p.type()
    << exit(FatalIOError);
  }
}

template<class Type>
symmetryPlaneFvsPatchField<Type>::symmetryPlaneFvsPatchField
(
  const symmetryPlaneFvsPatchField<Type>& ptf
)
:
  fvsPatchField<Type>{ptf}
{}

template<class Type>
symmetryPlaneFvsPatchField<Type>::symmetryPlaneFvsPatchField
(
  const symmetryPlaneFvsPatchField<Type>& ptf,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  fvsPatchField<Type>{ptf, iF}
{}

}  // namespace mousse
