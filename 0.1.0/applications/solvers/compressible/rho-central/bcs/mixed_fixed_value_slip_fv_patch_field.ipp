// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mixed_fixed_value_slip_fv_patch_field.hpp"
#include "symm_transform_field.hpp"


namespace mousse {

// Constructors 
template<class Type>
mixedFixedValueSlipFvPatchField<Type>::mixedFixedValueSlipFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  transformFvPatchField<Type>{p, iF},
  refValue_{p.size()},
  valueFraction_{p.size(), 1.0}
{}


template<class Type>
mixedFixedValueSlipFvPatchField<Type>::mixedFixedValueSlipFvPatchField
(
  const mixedFixedValueSlipFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  transformFvPatchField<Type>{ptf, p, iF, mapper},
  refValue_{ptf.refValue_, mapper},
  valueFraction_{ptf.valueFraction_, mapper}
{}


template<class Type>
mixedFixedValueSlipFvPatchField<Type>::mixedFixedValueSlipFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  transformFvPatchField<Type>{p, iF},
  refValue_{"refValue", dict, p.size()},
  valueFraction_{"valueFraction", dict, p.size()}
{}


template<class Type>
mixedFixedValueSlipFvPatchField<Type>::mixedFixedValueSlipFvPatchField
(
  const mixedFixedValueSlipFvPatchField<Type>& ptf
)
:
  transformFvPatchField<Type>{ptf},
  refValue_{ptf.refValue_},
  valueFraction_{ptf.valueFraction_}
{}


template<class Type>
mixedFixedValueSlipFvPatchField<Type>::mixedFixedValueSlipFvPatchField
(
  const mixedFixedValueSlipFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  transformFvPatchField<Type>{ptf, iF},
  refValue_{ptf.refValue_},
  valueFraction_{ptf.valueFraction_}
{}


// Member Functions 
// Map from self
template<class Type>
void mixedFixedValueSlipFvPatchField<Type>::autoMap
(
  const fvPatchFieldMapper& m
)
{
  Field<Type>::autoMap(m);
  refValue_.autoMap(m);
  valueFraction_.autoMap(m);
}


// Reverse-map the given fvPatchField onto this fvPatchField
template<class Type>
void mixedFixedValueSlipFvPatchField<Type>::rmap
(
  const fvPatchField<Type>& ptf,
  const labelList& addr
)
{
  transformFvPatchField<Type>::rmap(ptf, addr);
  const mixedFixedValueSlipFvPatchField<Type>& dmptf =
    refCast<const mixedFixedValueSlipFvPatchField<Type>>(ptf);
  refValue_.rmap(dmptf.refValue_, addr);
  valueFraction_.rmap(dmptf.valueFraction_, addr);
}


// Return gradient at boundary
template<class Type>
tmp<Field<Type>> mixedFixedValueSlipFvPatchField<Type>::snGrad() const
{
  tmp<vectorField> nHat = this->patch().nf();
  Field<Type> pif{this->patchInternalField()};
  return
  (
    valueFraction_*refValue_
    + (1.0 - valueFraction_)*transform(I - sqr(nHat), pif) - pif
  )*this->patch().deltaCoeffs();
}


// Evaluate the field on the patch
template<class Type>
void mixedFixedValueSlipFvPatchField<Type>::evaluate(const Pstream::commsTypes)
{
  if (!this->updated()) {
    this->updateCoeffs();
  }
  vectorField nHat{this->patch().nf()};
  Field<Type>::operator=
  (
    valueFraction_*refValue_
    + (1.0 - valueFraction_)
      *transform(I - nHat*nHat, this->patchInternalField())
  );
  transformFvPatchField<Type>::evaluate();
}


// Return defining fields
template<class Type>
tmp<Field<Type>>
mixedFixedValueSlipFvPatchField<Type>::snGradTransformDiag() const
{
  vectorField nHat{this->patch().nf()};
  vectorField diag{nHat.size()};
  diag.replace(vector::X, mag(nHat.component(vector::X)));
  diag.replace(vector::Y, mag(nHat.component(vector::Y)));
  diag.replace(vector::Z, mag(nHat.component(vector::Z)));
  return
    valueFraction_*Type(pTraits<Type>::one)
    + (1.0 - valueFraction_)
      *transformFieldMask<Type>(pow<vector, pTraits<Type>::rank>(diag));
}


// Write
template<class Type>
void mixedFixedValueSlipFvPatchField<Type>::write(Ostream& os) const
{
  transformFvPatchField<Type>::write(os);
  refValue_.writeEntry("refValue", os);
  valueFraction_.writeEntry("valueFraction", os);
}

}  // namespace mousse

