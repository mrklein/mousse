// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_fv_patch_field.hpp"
#include "transform_field.hpp"


namespace mousse {

// Constructors 
template<class Type>
cyclicFvPatchField<Type>::cyclicFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  coupledFvPatchField<Type>{p, iF},
  cyclicPatch_{refCast<const cyclicFvPatch>(p)}
{}


template<class Type>
cyclicFvPatchField<Type>::cyclicFvPatchField
(
  const cyclicFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  coupledFvPatchField<Type>{ptf, p, iF, mapper},
  cyclicPatch_{refCast<const cyclicFvPatch>(p)}
{
  if (!isA<cyclicFvPatch>(this->patch())) {
    FATAL_ERROR_IN
    (
      "cyclicFvPatchField<Type>::cyclicFvPatchField"
      "("
      "  const cyclicFvPatchField<Type>& ,"
      "  const fvPatch&, "
      "  const DimensionedField<Type, volMesh>&, "
      "  const fvPatchFieldMapper&"
      ")"
    )
    << "    patch type '" << p.type()
    << "' not constraint type '" << typeName << "'"
    << "\n    for patch " << p.name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << exit(FatalIOError);
  }
}


template<class Type>
cyclicFvPatchField<Type>::cyclicFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  coupledFvPatchField<Type>{p, iF, dict},
  cyclicPatch_{refCast<const cyclicFvPatch>(p)}
{
  if (!isA<cyclicFvPatch>(p)) {
    FATAL_IO_ERROR_IN
    (
      "cyclicFvPatchField<Type>::cyclicFvPatchField"
      "("
        "const fvPatch&, "
        "const Field<Type>&, "
        "const dictionary&"
      ")",
      dict
    )
    << "    patch type '" << p.type()
    << "' not constraint type '" << typeName << "'"
    << "\n    for patch " << p.name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << exit(FatalIOError);
  }
  this->evaluate(Pstream::blocking);
}


template<class Type>
cyclicFvPatchField<Type>::cyclicFvPatchField
(
  const cyclicFvPatchField<Type>& ptf
)
:
  cyclicLduInterfaceField{},
  coupledFvPatchField<Type>{ptf},
  cyclicPatch_{ptf.cyclicPatch_}
{}


template<class Type>
cyclicFvPatchField<Type>::cyclicFvPatchField
(
  const cyclicFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  coupledFvPatchField<Type>{ptf, iF},
  cyclicPatch_{ptf.cyclicPatch_}
{}


// Member Functions 
template<class Type>
tmp<Field<Type>> cyclicFvPatchField<Type>::patchNeighbourField() const
{
  const Field<Type>& iField = this->internalField();
  const labelUList& nbrFaceCells =
    cyclicPatch().cyclicPatch().neighbPatch().faceCells();
  tmp<Field<Type>> tpnf{new Field<Type>{this->size()}};
  Field<Type>& pnf = tpnf();
  if (doTransform()) {
    FOR_ALL(pnf, facei) {
      pnf[facei] = transform
      (
        forwardT()[0], iField[nbrFaceCells[facei]]
      );
    }
  } else {
    FOR_ALL(pnf, facei) {
      pnf[facei] = iField[nbrFaceCells[facei]];
    }
  }
  return tpnf;
}


template<class Type>
const cyclicFvPatchField<Type>& cyclicFvPatchField<Type>::neighbourPatchField()
const
{
  const GeometricField<Type, fvPatchField, volMesh>& fld =
    static_cast<const GeometricField<Type, fvPatchField, volMesh>&>
    (
      this->internalField()
    );
  return refCast<const cyclicFvPatchField<Type>>
  (
    fld.boundaryField()[this->cyclicPatch().neighbPatchID()]
  );
}


template<class Type>
void cyclicFvPatchField<Type>::updateInterfaceMatrix
(
  scalarField& result,
  const scalarField& psiInternal,
  const scalarField& coeffs,
  const direction cmpt,
  const Pstream::commsTypes
) const
{
  const labelUList& nbrFaceCells =
    cyclicPatch().cyclicPatch().neighbPatch().faceCells();
  scalarField pnf{psiInternal, nbrFaceCells};
  // Transform according to the transformation tensors
  transformCoupleField(pnf, cmpt);
  // Multiply the field by coefficients and add into the result
  const labelUList& faceCells = cyclicPatch_.faceCells();
  FOR_ALL(faceCells, elemI) {
    result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
  }
}


template<class Type>
void cyclicFvPatchField<Type>::updateInterfaceMatrix
(
  Field<Type>& result,
  const Field<Type>& psiInternal,
  const scalarField& coeffs,
  const Pstream::commsTypes
) const
{
  const labelUList& nbrFaceCells =
    cyclicPatch().cyclicPatch().neighbPatch().faceCells();
  Field<Type> pnf{psiInternal, nbrFaceCells};
  // Transform according to the transformation tensors
  transformCoupleField(pnf);
  // Multiply the field by coefficients and add into the result
  const labelUList& faceCells = cyclicPatch_.faceCells();
  FOR_ALL(faceCells, elemI) {
    result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
  }
}


template<class Type>
void cyclicFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
}

}  // namespace mousse

