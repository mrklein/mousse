// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_normal_slip_fv_patch_field.hpp"
#include "symm_transform_field.hpp"
// Constructors 
template<class Type>
mousse::fixedNormalSlipFvPatchField<Type>::fixedNormalSlipFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  transformFvPatchField<Type>(p, iF),
  fixedValue_(p.size(), pTraits<Type>::zero)
{}
template<class Type>
mousse::fixedNormalSlipFvPatchField<Type>::fixedNormalSlipFvPatchField
(
  const fixedNormalSlipFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  transformFvPatchField<Type>(ptf, p, iF, mapper),
  fixedValue_(ptf.fixedValue_, mapper)
{}
template<class Type>
mousse::fixedNormalSlipFvPatchField<Type>::fixedNormalSlipFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  transformFvPatchField<Type>(p, iF),
  fixedValue_("fixedValue", dict, p.size())
{
  evaluate();
}
template<class Type>
mousse::fixedNormalSlipFvPatchField<Type>::fixedNormalSlipFvPatchField
(
  const fixedNormalSlipFvPatchField<Type>& ptf
)
:
  transformFvPatchField<Type>(ptf),
  fixedValue_(ptf.fixedValue_)
{}
template<class Type>
mousse::fixedNormalSlipFvPatchField<Type>::fixedNormalSlipFvPatchField
(
  const fixedNormalSlipFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  transformFvPatchField<Type>(ptf, iF),
  fixedValue_(ptf.fixedValue_)
{}
// Member Functions 
template<class Type>
void mousse::fixedNormalSlipFvPatchField<Type>::autoMap
(
  const fvPatchFieldMapper& m
)
{
  transformFvPatchField<Type>::autoMap(m);
  fixedValue_.autoMap(m);
}
template<class Type>
void mousse::fixedNormalSlipFvPatchField<Type>::rmap
(
  const fvPatchField<Type>& ptf,
  const labelList& addr
)
{
  transformFvPatchField<Type>::rmap(ptf, addr);
  const fixedNormalSlipFvPatchField<Type>& dmptf =
    refCast<const fixedNormalSlipFvPatchField<Type> >(ptf);
  fixedValue_.rmap(dmptf.fixedValue_, addr);
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::fixedNormalSlipFvPatchField<Type>::snGrad() const
{
  const vectorField nHat(this->patch().nf());
  const Field<Type> pif(this->patchInternalField());
  return
  (
    (nHat*(nHat & fixedValue_) + transform(I - sqr(nHat), pif)) - pif
  )*this->patch().deltaCoeffs();
}
template<class Type>
void mousse::fixedNormalSlipFvPatchField<Type>::evaluate
(
  const Pstream::commsTypes
)
{
  if (!this->updated())
  {
    this->updateCoeffs();
  }
  const vectorField nHat(this->patch().nf());
  Field<Type>::operator=
  (
    nHat*(nHat & fixedValue_)
   + transform(I - sqr(nHat), this->patchInternalField())
  );
  transformFvPatchField<Type>::evaluate();
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::fixedNormalSlipFvPatchField<Type>::snGradTransformDiag() const
{
  const vectorField nHat(this->patch().nf());
  vectorField diag(nHat.size());
  diag.replace(vector::X, mag(nHat.component(vector::X)));
  diag.replace(vector::Y, mag(nHat.component(vector::Y)));
  diag.replace(vector::Z, mag(nHat.component(vector::Z)));
  return transformFieldMask<Type>(pow<vector, pTraits<Type>::rank>(diag));
}
template<class Type>
void mousse::fixedNormalSlipFvPatchField<Type>::write(Ostream& os) const
{
  transformFvPatchField<Type>::write(os);
  fixedValue_.writeEntry("fixedValue", os);
}
