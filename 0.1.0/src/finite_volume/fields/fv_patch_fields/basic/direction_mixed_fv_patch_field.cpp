// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "direction_mixed_fv_patch_field.hpp"
#include "symm_transform_field.hpp"
// Constructors 
template<class Type>
mousse::directionMixedFvPatchField<Type>::directionMixedFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  transformFvPatchField<Type>(p, iF),
  refValue_(p.size()),
  refGrad_(p.size()),
  valueFraction_(p.size())
{}
template<class Type>
mousse::directionMixedFvPatchField<Type>::directionMixedFvPatchField
(
  const directionMixedFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  transformFvPatchField<Type>(ptf, p, iF, mapper),
  refValue_(ptf.refValue_, mapper),
  refGrad_(ptf.refGrad_, mapper),
  valueFraction_(ptf.valueFraction_, mapper)
{
  if (notNull(iF) && mapper.hasUnmapped())
  {
    WarningIn
    (
      "directionMixedFvPatchField<Type>::directionMixedFvPatchField\n"
      "(\n"
      "    const directionMixedFvPatchField<Type>&,\n"
      "    const fvPatch&,\n"
      "    const DimensionedField<Type, volMesh>&,\n"
      "    const fvPatchFieldMapper&\n"
      ")\n"
    )   << "On field " << iF.name() << " patch " << p.name()
      << " patchField " << this->type()
      << " : mapper does not map all values." << nl
      << "    To avoid this warning fully specify the mapping in derived"
      << " patch fields." << endl;
  }
}
template<class Type>
mousse::directionMixedFvPatchField<Type>::directionMixedFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  transformFvPatchField<Type>(p, iF, dict),
  refValue_("refValue", dict, p.size()),
  refGrad_("refGradient", dict, p.size()),
  valueFraction_("valueFraction", dict, p.size())
{
  evaluate();
}
template<class Type>
mousse::directionMixedFvPatchField<Type>::directionMixedFvPatchField
(
  const directionMixedFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  transformFvPatchField<Type>(ptf, iF),
  refValue_(ptf.refValue_),
  refGrad_(ptf.refGrad_),
  valueFraction_(ptf.valueFraction_)
{}
// Member Functions 
template<class Type>
void mousse::directionMixedFvPatchField<Type>::autoMap
(
  const fvPatchFieldMapper& m
)
{
  transformFvPatchField<Type>::autoMap(m);
  refValue_.autoMap(m);
  refGrad_.autoMap(m);
  valueFraction_.autoMap(m);
}
template<class Type>
void mousse::directionMixedFvPatchField<Type>::rmap
(
  const fvPatchField<Type>& ptf,
  const labelList& addr
)
{
  transformFvPatchField<Type>::rmap(ptf, addr);
  const directionMixedFvPatchField<Type>& dmptf =
    refCast<const directionMixedFvPatchField<Type> >(ptf);
  refValue_.rmap(dmptf.refValue_, addr);
  refGrad_.rmap(dmptf.refGrad_, addr);
  valueFraction_.rmap(dmptf.valueFraction_, addr);
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::directionMixedFvPatchField<Type>::snGrad() const
{
  const Field<Type> pif(this->patchInternalField());
  tmp<Field<Type> > normalValue = transform(valueFraction_, refValue_);
  tmp<Field<Type> > gradValue = pif + refGrad_/this->patch().deltaCoeffs();
  tmp<Field<Type> > transformGradValue =
    transform(I - valueFraction_, gradValue);
  return
    (normalValue + transformGradValue - pif)*
    this->patch().deltaCoeffs();
}
template<class Type>
void mousse::directionMixedFvPatchField<Type>::evaluate(const Pstream::commsTypes)
{
  if (!this->updated())
  {
    this->updateCoeffs();
  }
  tmp<Field<Type> > normalValue = transform(valueFraction_, refValue_);
  tmp<Field<Type> > gradValue =
    this->patchInternalField() + refGrad_/this->patch().deltaCoeffs();
  tmp<Field<Type> > transformGradValue =
    transform(I - valueFraction_, gradValue);
  Field<Type>::operator=(normalValue + transformGradValue);
  transformFvPatchField<Type>::evaluate();
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::directionMixedFvPatchField<Type>::snGradTransformDiag() const
{
  vectorField diag(valueFraction_.size());
  diag.replace
  (
    vector::X,
    sqrt(mag(valueFraction_.component(symmTensor::XX)))
  );
  diag.replace
  (
    vector::Y,
    sqrt(mag(valueFraction_.component(symmTensor::YY)))
  );
  diag.replace
  (
    vector::Z,
    sqrt(mag(valueFraction_.component(symmTensor::ZZ)))
  );
  return transformFieldMask<Type>(pow<vector, pTraits<Type>::rank>(diag));
}
template<class Type>
void mousse::directionMixedFvPatchField<Type>::write(Ostream& os) const
{
  transformFvPatchField<Type>::write(os);
  refValue_.writeEntry("refValue", os);
  refGrad_.writeEntry("refGradient", os);
  valueFraction_.writeEntry("valueFraction", os);
  this->writeEntry("value", os);
}
