// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "empty_fv_patch_field.hpp"
#include "fv_patch_field_mapper.hpp"


namespace mousse {

// Constructors 
template<class Type>
emptyFvPatchField<Type>::emptyFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fvPatchField<Type>{p, iF, Field<Type>{0}}
{}


template<class Type>
emptyFvPatchField<Type>::emptyFvPatchField
(
  const emptyFvPatchField<Type>&,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper&
)
:
  fvPatchField<Type>{p, iF, Field<Type>{0}}
{
  if (!isType<emptyFvPatch>(p)) {
    FATAL_ERROR_IN
    (
      "emptyFvPatchField<Type>::emptyFvPatchField\n"
      "(\n"
      "    const emptyFvPatchField<Type>&,\n"
      "    const fvPatch& p,\n"
      "    const DimensionedField<Type, volMesh>& iF,\n"
      "    const fvPatchFieldMapper& mapper\n"
      ")\n"
    )
    << "\n    patch type '" << p.type()
    << "' not constraint type '" << typeName << "'"
    << "\n    for patch " << p.name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << exit(FatalIOError);
  }
}


template<class Type>
emptyFvPatchField<Type>::emptyFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fvPatchField<Type>{p, iF, Field<Type>{0}}
{
  if (!isType<emptyFvPatch>(p)) {
    FATAL_IO_ERROR_IN
    (
      "emptyFvPatchField<Type>::emptyFvPatchField\n"
      "(\n"
      "    const fvPatch& p,\n"
      "    const Field<Type>& field,\n"
      "    const dictionary& dict\n"
      ")\n",
      dict
    )
    << "\n    patch type '" << p.type()
    << "' not constraint type '" << typeName << "'"
    << "\n    for patch " << p.name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << exit(FatalIOError);
  }
}


template<class Type>
emptyFvPatchField<Type>::emptyFvPatchField
(
  const emptyFvPatchField<Type>& ptf
)
:
  fvPatchField<Type>
  {
    ptf.patch(),
    ptf.dimensionedInternalField(),
    Field<Type>{0}
  }
{}


template<class Type>
emptyFvPatchField<Type>::emptyFvPatchField
(
  const emptyFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fvPatchField<Type>{ptf.patch(), iF, Field<Type>{0}}
{}


// Member Functions 
template<class Type>
void emptyFvPatchField<Type>::updateCoeffs()
{
  //- Check moved to checkMesh. Test here breaks down if multiple empty
  //  patches.
}

}  // namespace mousse

