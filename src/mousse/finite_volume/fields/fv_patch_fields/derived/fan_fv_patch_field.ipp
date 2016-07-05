// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fan_fv_patch_field.hpp"


// Private Member Functions 
template<class Type>
void mousse::fanFvPatchField<Type>::calcFanJump()
{
  if (this->cyclicPatch().owner()) {
    this->jump_ = this->jumpTable_->value(this->db().time().value());
  }
}


// Constructors 
template<class Type>
mousse::fanFvPatchField<Type>::fanFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  uniformJumpFvPatchField<Type>{p, iF},
  phiName_{"phi"},
  rhoName_{"rho"}
{}


template<class Type>
mousse::fanFvPatchField<Type>::fanFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  uniformJumpFvPatchField<Type>{p, iF, dict},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  rhoName_{dict.lookupOrDefault<word>("rho", "rho")}
{}


template<class Type>
mousse::fanFvPatchField<Type>::fanFvPatchField
(
  const fanFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  uniformJumpFvPatchField<Type>{ptf, p, iF, mapper},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_}
{}


template<class Type>
mousse::fanFvPatchField<Type>::fanFvPatchField
(
  const fanFvPatchField<Type>& ptf
)
:
  uniformJumpFvPatchField<Type>{ptf},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_}
{}


template<class Type>
mousse::fanFvPatchField<Type>::fanFvPatchField
(
  const fanFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  uniformJumpFvPatchField<Type>{ptf, iF},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_}
{}


// Member Functions 
template<class Type>
void mousse::fanFvPatchField<Type>::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  calcFanJump();
  // Call fixedJump variant - uniformJump will overwrite the jump value
  fixedJumpFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void mousse::fanFvPatchField<Type>::write(Ostream& os) const
{
  uniformJumpFvPatchField<Type>::write(os);
  this->template writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  this->template writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
}

