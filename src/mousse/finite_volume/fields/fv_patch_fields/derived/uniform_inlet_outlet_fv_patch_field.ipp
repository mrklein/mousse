// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_inlet_outlet_fv_patch_field.hpp"


// Constructors 
template<class Type>
mousse::uniformInletOutletFvPatchField<Type>::uniformInletOutletFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  mixedFvPatchField<Type>{p, iF},
  phiName_{"phi"}
{
  this->refValue() = pTraits<Type>::zero;
  this->refGrad() = pTraits<Type>::zero;
  this->valueFraction() = 0.0;
}


template<class Type>
mousse::uniformInletOutletFvPatchField<Type>::uniformInletOutletFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchField<Type>{p, iF},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  uniformInletValue_{DataEntry<Type>::New("uniformInletValue", dict)}
{
  this->refValue() =
    uniformInletValue_->value(this->db().time().timeOutputValue());
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
mousse::uniformInletOutletFvPatchField<Type>::uniformInletOutletFvPatchField
(
  const uniformInletOutletFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchField<Type>{p, iF},  // Don't map
  phiName_{ptf.phiName_},
  uniformInletValue_{ptf.uniformInletValue_, false}
{
  this->patchType() = ptf.patchType();
  // Evaluate refValue since not mapped
  this->refValue() =
    uniformInletValue_->value(this->db().time().timeOutputValue());
  this->refGrad() = pTraits<Type>::zero;
  this->valueFraction() = 0.0;
  // Initialize the patch value to the refValue
  fvPatchField<Type>::operator=(this->refValue());
  this->map(ptf, mapper);
}


template<class Type>
mousse::uniformInletOutletFvPatchField<Type>::uniformInletOutletFvPatchField
(
  const uniformInletOutletFvPatchField<Type>& ptf
)
:
  mixedFvPatchField<Type>{ptf},
  phiName_{ptf.phiName_},
  uniformInletValue_{ptf.uniformInletValue_, false}
{}


template<class Type>
mousse::uniformInletOutletFvPatchField<Type>::uniformInletOutletFvPatchField
(
  const uniformInletOutletFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  mixedFvPatchField<Type>{ptf, iF},
  phiName_{ptf.phiName_},
  uniformInletValue_{ptf.uniformInletValue_, false}
{}


// Member Functions 
template<class Type>
void mousse::uniformInletOutletFvPatchField<Type>::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  this->refValue() =
    uniformInletValue_->value(this->db().time().timeOutputValue());
  const Field<scalar>& phip =
    this->patch().template lookupPatchField<surfaceScalarField, scalar>
    (
      phiName_
    );
  this->valueFraction() = 1.0 - pos(phip);
  mixedFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void mousse::uniformInletOutletFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  if (phiName_ != "phi") {
    os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
  }
  this->uniformInletValue_->writeData(os);
  this->writeEntry("value", os);
}


// Member Operators 
template<class Type>
void mousse::uniformInletOutletFvPatchField<Type>::autoMap
(
  const fvPatchFieldMapper& m
)
{
  mixedFvPatchField<Type>::autoMap(m);
  // Override
  this->refValue() =
    uniformInletValue_->value(this->db().time().timeOutputValue());
}


template<class Type>
void mousse::uniformInletOutletFvPatchField<Type>::rmap
(
  const fvPatchField<Type>& ptf,
  const labelList& addr
)
{
  mixedFvPatchField<Type>::rmap(ptf, addr);
  // Override
  this->refValue() =
    uniformInletValue_->value(this->db().time().timeOutputValue());
}


// Member Operators 
template<class Type>
void mousse::uniformInletOutletFvPatchField<Type>::operator=
(
  const fvPatchField<Type>& ptf
)
{
  fvPatchField<Type>::operator=
  (
    this->valueFraction()*this->refValue() + (1 - this->valueFraction())*ptf
  );
}

