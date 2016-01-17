// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class Type>
mousse::cyclicAMIFvPatchField<Type>::cyclicAMIFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  cyclicAMILduInterfaceField{},
  coupledFvPatchField<Type>{p, iF},
  cyclicAMIPatch_{refCast<const cyclicAMIFvPatch>(p)}
{}

template<class Type>
mousse::cyclicAMIFvPatchField<Type>::cyclicAMIFvPatchField
(
  const cyclicAMIFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  cyclicAMILduInterfaceField{},
  coupledFvPatchField<Type>{ptf, p, iF, mapper},
  cyclicAMIPatch_{refCast<const cyclicAMIFvPatch>(p)}
{
  if (!isA<cyclicAMIFvPatch>(this->patch()))
  {
    FATAL_ERROR_IN
    (
      "cyclicAMIFvPatchField<Type>::cyclicAMIFvPatchField"
      "("
        "const cyclicAMIFvPatchField<Type>& ,"
        "const fvPatch&, "
        "const DimensionedField<Type, volMesh>&, "
        "const fvPatchFieldMapper&"
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
mousse::cyclicAMIFvPatchField<Type>::cyclicAMIFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  cyclicAMILduInterfaceField{},
  coupledFvPatchField<Type>{p, iF, dict},
  cyclicAMIPatch_{refCast<const cyclicAMIFvPatch>(p)}
{
  if (!isA<cyclicAMIFvPatch>(p))
  {
    FATAL_IO_ERROR_IN
    (
      "cyclicAMIFvPatchField<Type>::cyclicAMIFvPatchField"
      "("
        "const fvPatch&, "
        "const DimensionedField<Type, volMesh>&, "
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
  if (!dict.found("value") && this->coupled())
  {
    this->evaluate(Pstream::blocking);
  }
}


template<class Type>
mousse::cyclicAMIFvPatchField<Type>::cyclicAMIFvPatchField
(
  const cyclicAMIFvPatchField<Type>& ptf
)
:
  cyclicAMILduInterfaceField{},
  coupledFvPatchField<Type>{ptf},
  cyclicAMIPatch_{ptf.cyclicAMIPatch_}
{}


template<class Type>
mousse::cyclicAMIFvPatchField<Type>::cyclicAMIFvPatchField
(
  const cyclicAMIFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  cyclicAMILduInterfaceField{},
  coupledFvPatchField<Type>{ptf, iF},
  cyclicAMIPatch_{ptf.cyclicAMIPatch_}
{}


// Member Functions 
template<class Type>
bool mousse::cyclicAMIFvPatchField<Type>::coupled() const
{
  return cyclicAMIPatch_.coupled();
}


template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::cyclicAMIFvPatchField<Type>::patchNeighbourField() const
{
  const Field<Type>& iField = this->internalField();
  const labelUList& nbrFaceCells =
    cyclicAMIPatch_.cyclicAMIPatch().neighbPatch().faceCells();
  Field<Type> pnf{iField, nbrFaceCells};
  tmp<Field<Type> > tpnf;
  if (cyclicAMIPatch_.applyLowWeightCorrection())
  {
    tpnf = cyclicAMIPatch_.interpolate(pnf, this->patchInternalField()());
  }
  else
  {
    tpnf = cyclicAMIPatch_.interpolate(pnf);
  }
  if (doTransform())
  {
    tpnf() = transform(forwardT(), tpnf());
  }
  return tpnf;
}


template<class Type>
const mousse::cyclicAMIFvPatchField<Type>&
mousse::cyclicAMIFvPatchField<Type>::neighbourPatchField() const
{
  const GeometricField<Type, fvPatchField, volMesh>& fld =
    static_cast<const GeometricField<Type, fvPatchField, volMesh>&>
    (
      this->internalField()
    );
  return refCast<const cyclicAMIFvPatchField<Type> >
  (
    fld.boundaryField()[cyclicAMIPatch_.neighbPatchID()]
  );
}


template<class Type>
void mousse::cyclicAMIFvPatchField<Type>::updateInterfaceMatrix
(
  scalarField& result,
  const scalarField& psiInternal,
  const scalarField& coeffs,
  const direction cmpt,
  const Pstream::commsTypes
) const
{
  const labelUList& nbrFaceCells =
    cyclicAMIPatch_.cyclicAMIPatch().neighbPatch().faceCells();
  scalarField pnf{psiInternal, nbrFaceCells};
  // Transform according to the transformation tensors
  transformCoupleField(pnf, cmpt);
  if (cyclicAMIPatch_.applyLowWeightCorrection())
  {
    scalarField pif(psiInternal, cyclicAMIPatch_.faceCells());
    pnf = cyclicAMIPatch_.interpolate(pnf, pif);
  }
  else
  {
    pnf = cyclicAMIPatch_.interpolate(pnf);
  }
  // Multiply the field by coefficients and add into the result
  const labelUList& faceCells = cyclicAMIPatch_.faceCells();
  FOR_ALL(faceCells, elemI)
  {
    result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
  }
}


template<class Type>
void mousse::cyclicAMIFvPatchField<Type>::updateInterfaceMatrix
(
  Field<Type>& result,
  const Field<Type>& psiInternal,
  const scalarField& coeffs,
  const Pstream::commsTypes
) const
{
  const labelUList& nbrFaceCells =
    cyclicAMIPatch_.cyclicAMIPatch().neighbPatch().faceCells();
  Field<Type> pnf{psiInternal, nbrFaceCells};
  // Transform according to the transformation tensors
  transformCoupleField(pnf);
  if (cyclicAMIPatch_.applyLowWeightCorrection())
  {
    Field<Type> pif(psiInternal, cyclicAMIPatch_.faceCells());
    pnf = cyclicAMIPatch_.interpolate(pnf, pif);
  }
  else
  {
    pnf = cyclicAMIPatch_.interpolate(pnf);
  }
  // Multiply the field by coefficients and add into the result
  const labelUList& faceCells = cyclicAMIPatch_.faceCells();
  FOR_ALL(faceCells, elemI)
  {
    result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
  }
}


template<class Type>
void mousse::cyclicAMIFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  this->writeEntry("value", os);
}
