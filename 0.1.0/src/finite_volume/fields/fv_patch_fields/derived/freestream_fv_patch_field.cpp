// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "freestream_fv_patch_field.hpp"
namespace mousse
{
// Constructors 
template<class Type>
freestreamFvPatchField<Type>::freestreamFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  inletOutletFvPatchField<Type>(p, iF)
{}
template<class Type>
freestreamFvPatchField<Type>::freestreamFvPatchField
(
  const freestreamFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  inletOutletFvPatchField<Type>(ptf, p, iF, mapper)
{}
template<class Type>
freestreamFvPatchField<Type>::freestreamFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  inletOutletFvPatchField<Type>(p, iF)
{
  this->phiName_ = dict.lookupOrDefault<word>("phi","phi");
  freestreamValue() = Field<Type>("freestreamValue", dict, p.size());
  if (dict.found("value"))
  {
    fvPatchField<Type>::operator=
    (
      Field<Type>("value", dict, p.size())
    );
  }
  else
  {
    fvPatchField<Type>::operator=(freestreamValue());
  }
}
template<class Type>
freestreamFvPatchField<Type>::freestreamFvPatchField
(
  const freestreamFvPatchField<Type>& ptf
)
:
  inletOutletFvPatchField<Type>(ptf)
{}
template<class Type>
freestreamFvPatchField<Type>::freestreamFvPatchField
(
  const freestreamFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  inletOutletFvPatchField<Type>(ptf, iF)
{}
// Member Functions 
template<class Type>
void freestreamFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  if (this->phiName_ != "phi")
  {
    os.writeKeyword("phi")
      << this->phiName_ << token::END_STATEMENT << nl;
  }
  freestreamValue().writeEntry("freestreamValue", os);
  this->writeEntry("value", os);
}
}  // namespace mousse
