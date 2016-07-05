// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_normal_slip_point_patch_field.hpp"


// Constructors 
template<class Type>
mousse::fixedNormalSlipPointPatchField<Type>::fixedNormalSlipPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  slipPointPatchField<Type>{p, iF},
  n_{vector::max}
{}


template<class Type>
mousse::fixedNormalSlipPointPatchField<Type>::fixedNormalSlipPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  slipPointPatchField<Type>{p, iF, dict},
  n_{dict.lookup("n")}
{}


template<class Type>
mousse::fixedNormalSlipPointPatchField<Type>::fixedNormalSlipPointPatchField
(
  const fixedNormalSlipPointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  slipPointPatchField<Type>{ptf, p, iF, mapper},
  n_{ptf.n_}
{}


template<class Type>
mousse::fixedNormalSlipPointPatchField<Type>::fixedNormalSlipPointPatchField
(
  const fixedNormalSlipPointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  slipPointPatchField<Type>{ptf, iF},
  n_{ptf.n_}
{}


// Member Functions 
template<class Type>
void mousse::fixedNormalSlipPointPatchField<Type>::evaluate
(
  const Pstream::commsTypes
)
{
  tmp<Field<Type>> tvalues = transform(I - n_*n_, this->patchInternalField());
  // Get internal field to insert values into
  Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());
  this->setInInternalField(iF, tvalues());
}


template<class Type>
void mousse::fixedNormalSlipPointPatchField<Type>::write(Ostream& os) const
{
  slipPointPatchField<Type>::write(os);
  os.writeKeyword("n") << n_ << token::END_STATEMENT << nl;
}

