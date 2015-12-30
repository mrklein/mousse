// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_mean_fv_patch_field.hpp"
#include "vol_fields.hpp"
// Constructors 
template<class Type>
mousse::fixedMeanFvPatchField<Type>::fixedMeanFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>(p, iF),
  meanValue_(pTraits<Type>::zero)
{}
template<class Type>
mousse::fixedMeanFvPatchField<Type>::fixedMeanFvPatchField
(
  const fixedMeanFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<Type>(ptf, p, iF, mapper),
  meanValue_(ptf.meanValue_)
{}
template<class Type>
mousse::fixedMeanFvPatchField<Type>::fixedMeanFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<Type>(p, iF, dict),
  meanValue_(pTraits<Type>(dict.lookup("meanValue")))
{}
template<class Type>
mousse::fixedMeanFvPatchField<Type>::fixedMeanFvPatchField
(
  const fixedMeanFvPatchField<Type>& ptf
)
:
  fixedValueFvPatchField<Type>(ptf),
  meanValue_(ptf.meanValue_)
{}
template<class Type>
mousse::fixedMeanFvPatchField<Type>::fixedMeanFvPatchField
(
  const fixedMeanFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>(ptf, iF),
  meanValue_(ptf.meanValue_)
{}
// Member Functions 
template<class Type>
void mousse::fixedMeanFvPatchField<Type>::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  Field<Type> newValues(this->patchInternalField());
  Type meanValuePsi =
    gSum(this->patch().magSf()*newValues)
   /gSum(this->patch().magSf());
  if (mag(meanValue_) > SMALL && mag(meanValuePsi)/mag(meanValue_) > 0.5)
  {
    newValues *= mag(meanValue_)/mag(meanValuePsi);
  }
  else
  {
    newValues += (meanValue_ - meanValuePsi);
  }
  this->operator==(newValues);
  fixedValueFvPatchField<Type>::updateCoeffs();
}
template<class Type>
void mousse::fixedMeanFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  os.writeKeyword("meanValue") << meanValue_ << token::END_STATEMENT << nl;
  this->writeEntry("value", os);
}
