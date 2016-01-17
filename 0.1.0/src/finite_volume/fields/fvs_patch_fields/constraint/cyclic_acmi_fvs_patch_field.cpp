// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_acmi_fvs_patch_field.hpp"

// Constructors 
template<class Type>
mousse::cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  coupledFvsPatchField<Type>{p, iF},
  cyclicACMIPatch_{refCast<const cyclicACMIFvPatch>(p)}
{}

template<class Type>
mousse::cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField
(
  const cyclicACMIFvsPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  coupledFvsPatchField<Type>{ptf, p, iF, mapper},
  cyclicACMIPatch_{refCast<const cyclicACMIFvPatch>(p)}
{
  if (!isA<cyclicACMIFvPatch>(this->patch()))
  {
    FATAL_ERROR_IN
    (
      "cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField\n"
      "("
        "const cyclicACMIFvsPatchField<Type>&, "
        "const fvPatch&, "
        "const DimensionedField<Type, surfaceMesh>&, "
        "const fvPatchFieldMapper&"
      ")"
    )
    << "Field type does not correspond to patch type for patch "
    << this->patch().index() << "." << endl
    << "Field type: " << typeName << endl
    << "Patch type: " << this->patch().type()
    << exit(FatalError);
  }
}

template<class Type>
mousse::cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const dictionary& dict
)
:
  coupledFvsPatchField<Type>{p, iF, dict},
  cyclicACMIPatch_{refCast<const cyclicACMIFvPatch>(p)}
{
  if (!isA<cyclicACMIFvPatch>(p))
  {
    FATAL_IO_ERROR_IN
    (
      "cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField"
      "("
        "const fvPatch&, "
        "const Field<Type>&, "
        "const dictionary&"
      ")",
      dict
    )
    << "patch " << this->patch().index() << " not cyclicACMI type. "
    << "Patch type = " << p.type()
    << exit(FatalIOError);
  }
}

template<class Type>
mousse::cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField
(
  const cyclicACMIFvsPatchField<Type>& ptf
)
:
  coupledFvsPatchField<Type>{ptf},
  cyclicACMIPatch_{ptf.cyclicACMIPatch_}
{}

template<class Type>
mousse::cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField
(
  const cyclicACMIFvsPatchField<Type>& ptf,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  coupledFvsPatchField<Type>{ptf, iF},
  cyclicACMIPatch_{ptf.cyclicACMIPatch_}
{}

// Member Functions 
template<class Type>
bool mousse::cyclicACMIFvsPatchField<Type>::coupled() const
{
/*
  if
  (
    Pstream::parRun()
  || (
      this->cyclicACMIPatch_.size()
    && this->cyclicACMIPatch_.cyclicACMIPatch().neighbPatch().size()
    )
  )
  {
    return true;
  }
  else
  {
    return false;
  }
*/
  return
    (Pstream::parRun()
     || (this->cyclicACMIPatch_.size()
         && this->cyclicACMIPatch_.cyclicACMIPatch().neighbPatch().size()));
}
