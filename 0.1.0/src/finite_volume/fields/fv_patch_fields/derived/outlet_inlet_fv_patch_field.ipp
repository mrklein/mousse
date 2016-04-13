// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "outlet_inlet_fv_patch_field.hpp"


// Constructors 
template<class Type>
mousse::outletInletFvPatchField<Type>::outletInletFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  mixedFvPatchField<Type>{p, iF},
  phiName_{"phi"}
{
  this->refValue() = *this;
  this->refGrad() = pTraits<Type>::zero;
  this->valueFraction() = 0.0;
}


template<class Type>
mousse::outletInletFvPatchField<Type>::outletInletFvPatchField
(
  const outletInletFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchField<Type>{ptf, p, iF, mapper},
  phiName_{ptf.phiName_}
{}


template<class Type>
mousse::outletInletFvPatchField<Type>::outletInletFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchField<Type>{p, iF},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")}
{
  this->refValue() = Field<Type>("outletValue", dict, p.size());
  if (dict.found("value")) {
    fvPatchField<Type>::operator=
    (
      Field<Type>{"value", dict, p.size()}
    );
  } else {
    fvPatchField<Type>::operator=(this->refValue());
  }
  this->refGrad() = pTraits<Type>::zero;
  this->valueFraction() = 0.0;
}


template<class Type>
mousse::outletInletFvPatchField<Type>::outletInletFvPatchField
(
  const outletInletFvPatchField<Type>& ptf
)
:
  mixedFvPatchField<Type>{ptf},
  phiName_{ptf.phiName_}
{}


template<class Type>
mousse::outletInletFvPatchField<Type>::outletInletFvPatchField
(
  const outletInletFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  mixedFvPatchField<Type>{ptf, iF},
  phiName_{ptf.phiName_}
{}


// Member Functions 
template<class Type>
void mousse::outletInletFvPatchField<Type>::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  const Field<scalar>& phip =
    this->patch().template lookupPatchField<surfaceScalarField, scalar>
    (
      phiName_
    );
  this->valueFraction() = pos(phip);
  mixedFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void mousse::outletInletFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  if (phiName_ != "phi") {
    os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
  }
  this->refValue().writeEntry("outletValue", os);
  this->writeEntry("value", os);
}

