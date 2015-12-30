// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_field_fv_patch_field.hpp"
#include "vol_fields.hpp"
#include "interpolation_cell.hpp"
namespace mousse
{
// Constructors 
template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>(p, iF),
  mappedPatchBase(p.patch()),
  mappedPatchFieldBase<Type>(*this, *this)
{}
template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
  const mappedFieldFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<Type>(ptf, p, iF, mapper),
  mappedPatchBase(p.patch(), ptf),
  mappedPatchFieldBase<Type>(*this, *this, ptf)
{}
template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<Type>(p, iF, dict),
  mappedPatchBase(p.patch(), dict),
  mappedPatchFieldBase<Type>(*this, *this, dict)
{}
template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  // mappedPatchBase
  const word& sampleRegion,
  const sampleMode sampleMode,
  const word& samplePatch,
  const scalar distance,
  // My settings
  const word& fieldName,
  const bool setAverage,
  const Type average,
  const word& interpolationScheme
)
:
  fixedValueFvPatchField<Type>(p, iF),
  mappedPatchBase
  (
    p.patch(),
    sampleRegion,
    sampleMode,
    samplePatch,
    distance
  ),
  mappedPatchFieldBase<Type>
  (
    *this,
    *this,
    fieldName,
    setAverage,
    average,
    interpolationScheme
  )
{}
template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
  const mappedFieldFvPatchField<Type>& ptf
)
:
  fixedValueFvPatchField<Type>(ptf),
  mappedPatchBase(ptf.patch().patch(), ptf),
  mappedPatchFieldBase<Type>(ptf)
{}
template<class Type>
mappedFieldFvPatchField<Type>::mappedFieldFvPatchField
(
  const mappedFieldFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>(ptf, iF),
  mappedPatchBase(ptf.patch().patch(), ptf),
  mappedPatchFieldBase<Type>(*this, *this, ptf)
{}
// Member Functions 
template<class Type>
void mappedFieldFvPatchField<Type>::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  this->operator==(this->mappedField());
  if (debug)
  {
    Info<< "operating on field:" << this->dimensionedInternalField().name()
      << " patch:" << this->patch().name()
      << "  avg:" << gAverage(*this)
      << "  min:" << gMin(*this)
      << "  max:" << gMax(*this)
      << endl;
  }
  fixedValueFvPatchField<Type>::updateCoeffs();
}
template<class Type>
void mappedFieldFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  mappedPatchBase::write(os);
  mappedPatchFieldBase<Type>::write(os);
  this->writeEntry("value", os);
}
}  // namespace mousse
