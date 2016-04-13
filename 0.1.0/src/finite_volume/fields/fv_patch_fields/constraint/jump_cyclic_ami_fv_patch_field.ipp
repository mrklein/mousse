// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "jump_cyclic_ami_fv_patch_field.hpp"
#include "transform_field.hpp"


// Constructors 
template<class Type>
mousse::jumpCyclicAMIFvPatchField<Type>::jumpCyclicAMIFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  cyclicAMIFvPatchField<Type>{p, iF}
{}


template<class Type>
mousse::jumpCyclicAMIFvPatchField<Type>::jumpCyclicAMIFvPatchField
(
  const jumpCyclicAMIFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  cyclicAMIFvPatchField<Type>{ptf, p, iF, mapper}
{}


template<class Type>
mousse::jumpCyclicAMIFvPatchField<Type>::jumpCyclicAMIFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  cyclicAMIFvPatchField<Type>{p, iF, dict}
{
  // Call this evaluation in derived classes
  //this->evaluate(Pstream::blocking);
}


template<class Type>
mousse::jumpCyclicAMIFvPatchField<Type>::jumpCyclicAMIFvPatchField
(
  const jumpCyclicAMIFvPatchField<Type>& ptf
)
:
  cyclicAMIFvPatchField<Type>{ptf}
{}


template<class Type>
mousse::jumpCyclicAMIFvPatchField<Type>::jumpCyclicAMIFvPatchField
(
  const jumpCyclicAMIFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  cyclicAMIFvPatchField<Type>{ptf, iF}
{}


// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::jumpCyclicAMIFvPatchField<Type>::patchNeighbourField() const
{
  const Field<Type>& iField = this->internalField();
  const labelUList& nbrFaceCells =
    this->cyclicAMIPatch().cyclicAMIPatch().neighbPatch().faceCells();
  Field<Type> pnf{iField, nbrFaceCells};
  tmp<Field<Type>> tpnf;
  if (this->cyclicAMIPatch().applyLowWeightCorrection()) {
    tpnf =
      this->cyclicAMIPatch().interpolate
      (
        pnf,
        this->patchInternalField()()
      );
  } else {
    tpnf = this->cyclicAMIPatch().interpolate(pnf);
  }
  if (this->doTransform()) {
    tpnf = transform(this->forwardT(), tpnf);
  }
  tmp<Field<Type> > tjf = jump();
  if (!this->cyclicAMIPatch().owner()) {
    tjf = -tjf;
  }
  return tpnf - tjf;
}


template<class Type>
void mousse::jumpCyclicAMIFvPatchField<Type>::updateInterfaceMatrix
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
    "void mousse::jumpCyclicAMIFvPatchField<Type>::updateInterfaceMatrix"
    "("
      "scalarField&, "
      "const scalarField&, "
      "const scalarField& coeffs,"
      "const direction, "
      "const Pstream::commsTypes"
    ") const"
  );
}


template<class Type>
void mousse::jumpCyclicAMIFvPatchField<Type>::updateInterfaceMatrix
(
  Field<Type>& result,
  const Field<Type>& psiInternal,
  const scalarField& coeffs,
  const Pstream::commsTypes
) const
{
  const labelUList& nbrFaceCells =
    this->cyclicAMIPatch().cyclicAMIPatch().neighbPatch().faceCells();
  Field<Type> pnf{psiInternal, nbrFaceCells};
  if (this->cyclicAMIPatch().applyLowWeightCorrection()) {
    pnf =
      this->cyclicAMIPatch().interpolate
      (
        pnf,
        this->patchInternalField()()
      );
  } else {
    pnf = this->cyclicAMIPatch().interpolate(pnf);
  }
  // only apply jump to original field
  if (&psiInternal == &this->internalField()) {
    Field<Type> jf{this->jump()};
    if (!this->cyclicAMIPatch().owner()) {
      jf *= -1.0;
    }
    pnf -= jf;
  }
  // Transform according to the transformation tensors
  this->transformCoupleField(pnf);
  // Multiply the field by coefficients and add into the result
  const labelUList& faceCells = this->cyclicAMIPatch().faceCells();
  FOR_ALL(faceCells, elemI) {
    result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
  }
}
