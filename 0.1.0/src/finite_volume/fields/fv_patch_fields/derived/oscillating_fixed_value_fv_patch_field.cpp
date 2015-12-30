// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "oscillating_fixed_value_fv_patch_field.hpp"
#include "mathematical_constants.hpp"
namespace mousse
{
// Private Member Functions 
template<class Type>
scalar oscillatingFixedValueFvPatchField<Type>::currentScale() const
{
  const scalar t = this->db().time().timeOutputValue();
  const scalar a = amplitude_->value(t);
  const scalar f = frequency_->value(t);
  return 1.0 + a*sin(constant::mathematical::twoPi*f*t);
}
// Constructors 
template<class Type>
oscillatingFixedValueFvPatchField<Type>::oscillatingFixedValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>(p, iF),
  refValue_(p.size()),
  offset_(pTraits<Type>::zero),
  amplitude_(),
  frequency_(),
  curTimeIndex_(-1)
{}
template<class Type>
oscillatingFixedValueFvPatchField<Type>::oscillatingFixedValueFvPatchField
(
  const oscillatingFixedValueFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<Type>(ptf, p, iF, mapper),
  refValue_(ptf.refValue_, mapper),
  offset_(ptf.offset_),
  amplitude_(ptf.amplitude_().clone().ptr()),
  frequency_(ptf.frequency_().clone().ptr()),
  curTimeIndex_(-1)
{}
template<class Type>
oscillatingFixedValueFvPatchField<Type>::oscillatingFixedValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<Type>(p, iF),
  refValue_("refValue", dict, p.size()),
  offset_(dict.lookupOrDefault<Type>("offset", pTraits<Type>::zero)),
  amplitude_(DataEntry<scalar>::New("amplitude", dict)),
  frequency_(DataEntry<scalar>::New("frequency", dict)),
  curTimeIndex_(-1)
{
  if (dict.found("value"))
  {
    fixedValueFvPatchField<Type>::operator==
    (
      Field<Type>("value", dict, p.size())
    );
  }
  else
  {
    fixedValueFvPatchField<Type>::operator==
    (
      refValue_*currentScale()
     + offset_
    );
  }
}
template<class Type>
oscillatingFixedValueFvPatchField<Type>::oscillatingFixedValueFvPatchField
(
  const oscillatingFixedValueFvPatchField<Type>& ptf
)
:
  fixedValueFvPatchField<Type>(ptf),
  refValue_(ptf.refValue_),
  offset_(ptf.offset_),
  amplitude_(ptf.amplitude_().clone().ptr()),
  frequency_(ptf.frequency_().clone().ptr()),
  curTimeIndex_(-1)
{}
template<class Type>
oscillatingFixedValueFvPatchField<Type>::oscillatingFixedValueFvPatchField
(
  const oscillatingFixedValueFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>(ptf, iF),
  refValue_(ptf.refValue_),
  offset_(ptf.offset_),
  amplitude_(ptf.amplitude_().clone().ptr()),
  frequency_(ptf.frequency_().clone().ptr()),
  curTimeIndex_(-1)
{}
// Member Functions 
template<class Type>
void oscillatingFixedValueFvPatchField<Type>::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedValueFvPatchField<Type>::autoMap(m);
  refValue_.autoMap(m);
}
template<class Type>
void oscillatingFixedValueFvPatchField<Type>::rmap
(
  const fvPatchField<Type>& ptf,
  const labelList& addr
)
{
  fixedValueFvPatchField<Type>::rmap(ptf, addr);
  const oscillatingFixedValueFvPatchField<Type>& tiptf =
    refCast<const oscillatingFixedValueFvPatchField<Type> >(ptf);
  refValue_.rmap(tiptf.refValue_, addr);
}
template<class Type>
void oscillatingFixedValueFvPatchField<Type>::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  if (curTimeIndex_ != this->db().time().timeIndex())
  {
    fixedValueFvPatchField<Type>::operator==
    (
      refValue_*currentScale()
     + offset_
    );
    curTimeIndex_ = this->db().time().timeIndex();
  }
  fixedValueFvPatchField<Type>::updateCoeffs();
}
template<class Type>
void oscillatingFixedValueFvPatchField<Type>::write(Ostream& os) const
{
  fixedValueFvPatchField<Type>::write(os);
  refValue_.writeEntry("refValue", os);
  os.writeKeyword("offset") << offset_ << token::END_STATEMENT << nl;
  amplitude_->writeData(os);
  frequency_->writeData(os);
}
}  // namespace mousse
