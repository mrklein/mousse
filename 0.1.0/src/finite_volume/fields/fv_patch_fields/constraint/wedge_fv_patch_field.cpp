// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wedge_fv_patch.hpp"
#include "wedge_fv_patch_field.hpp"
#include "transform_field.hpp"
#include "symm_transform.hpp"
#include "diag_tensor.hpp"
// Constructors 
template<class Type>
mousse::wedgeFvPatchField<Type>::wedgeFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  transformFvPatchField<Type>(p, iF)
{}
template<class Type>
mousse::wedgeFvPatchField<Type>::wedgeFvPatchField
(
  const wedgeFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  transformFvPatchField<Type>(ptf, p, iF, mapper)
{
  if (!isType<wedgeFvPatch>(this->patch()))
  {
    FatalErrorIn
    (
      "wedgeFvPatchField<Type>::wedgeFvPatchField\n"
      "(\n"
      "    const wedgeFvPatchField<Type>& ptf,\n"
      "    const fvPatch& p,\n"
      "    const DimensionedField<Type, volMesh>& iF,\n"
      "    const fvPatchFieldMapper& mapper\n"
      ")\n"
    )   << "\n    patch type '" << p.type()
      << "' not constraint type '" << typeName << "'"
      << "\n    for patch " << p.name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << exit(FatalIOError);
  }
}
template<class Type>
mousse::wedgeFvPatchField<Type>::wedgeFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  transformFvPatchField<Type>(p, iF, dict)
{
  if (!isType<wedgeFvPatch>(p))
  {
    FatalIOErrorIn
    (
      "wedgeFvPatchField<Type>::wedgeFvPatchField\n"
      "(\n"
      "    const fvPatch& p,\n"
      "    const Field<Type>& field,\n"
      "    dictionary& dict\n"
      ")\n",
      dict
    )   << "\n    patch type '" << p.type()
      << "' not constraint type '" << typeName << "'"
      << "\n    for patch " << p.name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << exit(FatalIOError);
  }
  evaluate();
}
template<class Type>
mousse::wedgeFvPatchField<Type>::wedgeFvPatchField
(
  const wedgeFvPatchField<Type>& ptf
)
:
  transformFvPatchField<Type>(ptf)
{}
template<class Type>
mousse::wedgeFvPatchField<Type>::wedgeFvPatchField
(
  const wedgeFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  transformFvPatchField<Type>(ptf, iF)
{}
// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::wedgeFvPatchField<Type>::snGrad() const
{
  const Field<Type> pif(this->patchInternalField());
  return
  (
    transform(refCast<const wedgeFvPatch>(this->patch()).cellT(), pif) - pif
  )*(0.5*this->patch().deltaCoeffs());
}
template<class Type>
void mousse::wedgeFvPatchField<Type>::evaluate(const Pstream::commsTypes)
{
  if (!this->updated())
  {
    this->updateCoeffs();
  }
  fvPatchField<Type>::operator==
  (
    transform
    (
      refCast<const wedgeFvPatch>(this->patch()).faceT(),
      this->patchInternalField()
    )
  );
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::wedgeFvPatchField<Type>::snGradTransformDiag() const
{
  const diagTensor diagT =
    0.5*diag(I - refCast<const wedgeFvPatch>(this->patch()).cellT());
  const vector diagV(diagT.xx(), diagT.yy(), diagT.zz());
  return tmp<Field<Type> >
  (
    new Field<Type>
    (
      this->size(),
      transformMask<Type>
      (
        pow
        (
          diagV,
          pTraits<typename powProduct<vector, pTraits<Type>::rank>
          ::type>::zero
        )
      )
    )
  );
}
