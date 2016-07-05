// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_ami_fvs_patch_field.hpp"


// Constructors 
template<class Type>
mousse::cyclicAMIFvsPatchField<Type>::cyclicAMIFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  coupledFvsPatchField<Type>{p, iF},
  cyclicAMIPatch_{refCast<const cyclicAMIFvPatch>(p)}
{}


template<class Type>
mousse::cyclicAMIFvsPatchField<Type>::cyclicAMIFvsPatchField
(
  const cyclicAMIFvsPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  coupledFvsPatchField<Type>{ptf, p, iF, mapper},
  cyclicAMIPatch_{refCast<const cyclicAMIFvPatch>(p)}
{
  if (!isA<cyclicAMIFvPatch>(this->patch())) {
    FATAL_ERROR_IN
    (
      "cyclicAMIFvsPatchField<Type>::cyclicAMIFvsPatchField\n"
      "("
        "const cyclicAMIFvsPatchField<Type>&, "
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
mousse::cyclicAMIFvsPatchField<Type>::cyclicAMIFvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const dictionary& dict
)
:
  coupledFvsPatchField<Type>{p, iF, dict},
  cyclicAMIPatch_{refCast<const cyclicAMIFvPatch>(p)}
{
  if (!isA<cyclicAMIFvPatch>(p)) {
    FATAL_IO_ERROR_IN
    (
      "cyclicAMIFvsPatchField<Type>::cyclicAMIFvsPatchField"
      "("
        "const fvPatch&, "
        "const Field<Type>&, "
        "const dictionary&"
      ")",
      dict
    )
    << "patch " << this->patch().index() << " not cyclicAMI type. "
    << "Patch type = " << p.type()
    << exit(FatalIOError);
  }
}


template<class Type>
mousse::cyclicAMIFvsPatchField<Type>::cyclicAMIFvsPatchField
(
  const cyclicAMIFvsPatchField<Type>& ptf
)
:
  coupledFvsPatchField<Type>{ptf},
  cyclicAMIPatch_{ptf.cyclicAMIPatch_}
{}


template<class Type>
mousse::cyclicAMIFvsPatchField<Type>::cyclicAMIFvsPatchField
(
  const cyclicAMIFvsPatchField<Type>& ptf,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  coupledFvsPatchField<Type>{ptf, iF},
  cyclicAMIPatch_{ptf.cyclicAMIPatch_}
{}


// Member Functions 
template<class Type>
bool mousse::cyclicAMIFvsPatchField<Type>::coupled() const
{
#if 0
  if (Pstream::parRun()
      || (this->cyclicAMIPatch_.size()
          && this->cyclicAMIPatch_.cyclicAMIPatch().neighbPatch().size()))
  {
    return true;
  }
  else
  {
    return false;
  }
#endif
  return
    (Pstream::parRun()
     || (this->cyclicAMIPatch_.size()
         && this->cyclicAMIPatch_.cyclicAMIPatch().neighbPatch().size()));
}

