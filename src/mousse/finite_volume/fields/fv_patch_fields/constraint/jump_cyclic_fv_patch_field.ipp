// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "jump_cyclic_fv_patch_field.hpp"


// Constructors 
template<class Type>
mousse::jumpCyclicFvPatchField<Type>::jumpCyclicFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  cyclicFvPatchField<Type>{p, iF}
{}


template<class Type>
mousse::jumpCyclicFvPatchField<Type>::jumpCyclicFvPatchField
(
  const jumpCyclicFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  cyclicFvPatchField<Type>{ptf, p, iF, mapper}
{}


template<class Type>
mousse::jumpCyclicFvPatchField<Type>::jumpCyclicFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  cyclicFvPatchField<Type>{p, iF, dict}
{
  // Call this evaluation in derived classes
  //this->evaluate(Pstream::blocking);
}


template<class Type>
mousse::jumpCyclicFvPatchField<Type>::jumpCyclicFvPatchField
(
  const jumpCyclicFvPatchField<Type>& ptf
)
:
  cyclicFvPatchField<Type>{ptf}
{}


template<class Type>
mousse::jumpCyclicFvPatchField<Type>::jumpCyclicFvPatchField
(
  const jumpCyclicFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  cyclicFvPatchField<Type>{ptf, iF}
{}


// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::jumpCyclicFvPatchField<Type>::patchNeighbourField() const
{
  const Field<Type>& iField = this->internalField();
  const labelUList& nbrFaceCells =
    this->cyclicPatch().neighbFvPatch().faceCells();
  tmp<Field<Type>> tpnf{new Field<Type>{this->size()}};
  Field<Type>& pnf = tpnf();
  Field<Type> jf(this->jump());
  if (!this->cyclicPatch().owner()) {
    jf *= -1.0;
  }
  if (this->doTransform()) {
    FOR_ALL(*this, facei) {
      pnf[facei] =
        transform(this->forwardT()[0], iField[nbrFaceCells[facei]]) - jf[facei];
    }
  } else {
    FOR_ALL(*this, facei) {
      pnf[facei] = iField[nbrFaceCells[facei]] - jf[facei];
    }
  }
  return tpnf;
}


template<class Type>
void mousse::jumpCyclicFvPatchField<Type>::updateInterfaceMatrix
(
  scalarField& /*result*/,
  const scalarField& /*psiInternal*/,
  const scalarField& /*coeffs*/,
  const direction /*cmpt*/,
  const Pstream::commsTypes
) const
{
  NOT_IMPLEMENTED
  (
    "void mousse::jumpCyclicFvPatchField<Type>::updateInterfaceMatrix"
    "("
      "scalarField&, "
      "const scalarField&, "
      "const scalarField&, "
      "const direction, "
      "const Pstream::commsTypes"
    ") const"
  );
}


template<class Type>
void mousse::jumpCyclicFvPatchField<Type>::updateInterfaceMatrix
(
  Field<Type>& result,
  const Field<Type>& psiInternal,
  const scalarField& coeffs,
  const Pstream::commsTypes
) const
{
  Field<Type> pnf{this->size()};
  const labelUList& nbrFaceCells =
    this->cyclicPatch().neighbFvPatch().faceCells();
  // only apply jump to original field
  if (&psiInternal == &this->internalField()) {
    Field<Type> jf{this->jump()};
    if (!this->cyclicPatch().owner()) {
      jf *= -1.0;
    }
    FOR_ALL(*this, facei) {
      pnf[facei] = psiInternal[nbrFaceCells[facei]] - jf[facei];
    }
  } else {
    FOR_ALL(*this, facei) {
      pnf[facei] = psiInternal[nbrFaceCells[facei]];
    }
  }
  // Transform according to the transformation tensors
  this->transformCoupleField(pnf);
  // Multiply the field by coefficients and add into the result
  const labelUList& faceCells = this->cyclicPatch().faceCells();
  FOR_ALL(faceCells, elemI) {
    result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
  }
}

