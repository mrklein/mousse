// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_fixed_internal_value_fv_patch_field.hpp"
#include "uindirect_list.hpp"


// Constructors 
template<class Type>
mousse::mappedFixedInternalValueFvPatchField<Type>::
mappedFixedInternalValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  mappedFixedValueFvPatchField<Type>{p, iF}
{}


template<class Type>
mousse::mappedFixedInternalValueFvPatchField<Type>::
mappedFixedInternalValueFvPatchField
(
  const mappedFixedInternalValueFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mappedFixedValueFvPatchField<Type>{ptf, p, iF, mapper}
{}


template<class Type>
mousse::mappedFixedInternalValueFvPatchField<Type>::
mappedFixedInternalValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  mappedFixedValueFvPatchField<Type>{p, iF, dict}
{}


template<class Type>
mousse::mappedFixedInternalValueFvPatchField<Type>::
mappedFixedInternalValueFvPatchField
(
  const mappedFixedInternalValueFvPatchField<Type>& ptf
)
:
  mappedFixedValueFvPatchField<Type>{ptf}
{}


template<class Type>
mousse::mappedFixedInternalValueFvPatchField<Type>::
mappedFixedInternalValueFvPatchField
(
  const mappedFixedInternalValueFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  mappedFixedValueFvPatchField<Type>{ptf, iF}
{}


// Member Functions 
template<class Type>
void mousse::mappedFixedInternalValueFvPatchField<Type>::updateCoeffs()
{
  typedef GeometricField<Type, fvPatchField, volMesh> FieldType;
  if (this->updated())
  {
    return;
  }
  // Since we're inside initEvaluate/evaluate there might be processor
  // comms underway. Change the tag we use.
  int oldTag = UPstream::msgType();
  UPstream::msgType() = oldTag + 1;
  // Retrieve the neighbour values and assign to this patch boundary field
  mappedFixedValueFvPatchField<Type>::updateCoeffs();
  // Get the coupling information from the mappedPatchBase
  const mappedPatchBase& mpp =
    refCast<const mappedPatchBase>(this->patch().patch());
  const fvMesh& nbrMesh = refCast<const fvMesh>(mpp.sampleMesh());
  Field<Type> nbrIntFld;
  switch (mpp.mode()) {
    case mappedPatchBase::NEARESTCELL:
    {
      FATAL_ERROR_IN
      (
        "void mappedFixedValueFvPatchField<Type>::updateCoeffs()"
      )
      << "Cannot apply "
      << mappedPatchBase::sampleModeNames_[mappedPatchBase::NEARESTCELL]
      << " mapping mode for patch " << this->patch().name()
      << exit(FatalError);
      break;
    }
    case mappedPatchBase::NEARESTPATCHFACE:
    case mappedPatchBase::NEARESTPATCHFACEAMI:
    {
      const label samplePatchI = mpp.samplePolyPatch().index();
      const fvPatchField<Type>& nbrPatchField =
        this->sampleField().boundaryField()[samplePatchI];
      nbrIntFld = nbrPatchField.patchInternalField();
      mpp.distribute(nbrIntFld);
      break;
    }
    case mappedPatchBase::NEARESTFACE:
    {
      Field<Type> allValues{nbrMesh.nFaces(), pTraits<Type>::zero};
      const FieldType& nbrField = this->sampleField();
      FOR_ALL(nbrField.boundaryField(), patchI) {
        const fvPatchField<Type>& pf = nbrField.boundaryField()[patchI];
        const Field<Type> pif{pf.patchInternalField()};
        label faceStart = pf.patch().start();
        FOR_ALL(pf, faceI) {
          allValues[faceStart++] = pif[faceI];
        }
      }
      mpp.distribute(allValues);
      nbrIntFld.transfer(allValues);
      break;
    }
    default:
    {
      FATAL_ERROR_IN("mappedFixedValueFvPatchField<Type>::updateCoeffs()")
        << "Unknown sampling mode: " << mpp.mode()
        << abort(FatalError);
    }
  }
  // Restore tag
  UPstream::msgType() = oldTag;
  // Assign to (this) patch internal field its neighbour values
  Field<Type>& intFld = const_cast<Field<Type>&>(this->internalField());
  UIndirectList<Type>(intFld, this->patch().faceCells()) = nbrIntFld;
}


template<class Type>
void mousse::mappedFixedInternalValueFvPatchField<Type>::write
(
  Ostream& os
) const
{
  mappedFixedValueFvPatchField<Type>::write(os);
}

