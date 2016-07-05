// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_cyclic_fv_patch_field.hpp"
#include "processor_cyclic_fv_patch.hpp"
#include "demand_driven_data.hpp"
#include "transform_field.hpp"


// Constructors
template<class Type>
mousse::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  processorFvPatchField<Type>{p, iF},
  procPatch_{refCast<const processorCyclicFvPatch>(p)}
{}


template<class Type>
mousse::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const Field<Type>& f
)
:
  processorFvPatchField<Type>{p, iF, f},
  procPatch_{refCast<const processorCyclicFvPatch>(p)}
{}


// Construct by mapping given processorCyclicFvPatchField<Type>
template<class Type>
mousse::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
  const processorCyclicFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  processorFvPatchField<Type>{ptf, p, iF, mapper},
  procPatch_{refCast<const processorCyclicFvPatch>(p)}
{
  if (!isType<processorCyclicFvPatch>(this->patch())) {
    FATAL_ERROR_IN
    (
      "processorCyclicFvPatchField<Type>::processorCyclicFvPatchField\n"
      "(\n"
      "    const processorCyclicFvPatchField<Type>& ptf,\n"
      "    const fvPatch& p,\n"
      "    const DimensionedField<Type, volMesh>& iF,\n"
      "    const fvPatchFieldMapper& mapper\n"
      ")\n"
    )
    << "\n    patch type '" << p.type()
    << "' not constraint type '" << typeName << "'"
    << "\n    for patch " << p.name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << exit(FatalIOError);
  }
}


template<class Type>
mousse::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  processorFvPatchField<Type>{p, iF, dict},
  procPatch_{refCast<const processorCyclicFvPatch>(p)}
{
  if (!isType<processorCyclicFvPatch>(p)) {
    FATAL_IO_ERROR_IN
    (
      "processorCyclicFvPatchField<Type>::processorCyclicFvPatchField\n"
      "(\n"
      "    const fvPatch& p,\n"
      "    const Field<Type>& field,\n"
      "    const dictionary& dict\n"
      ")\n",
      dict
    )
    << "\n    patch type '" << p.type()
    << "' not constraint type '" << typeName << "'"
    << "\n    for patch " << p.name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << exit(FatalIOError);
  }
  if (Pstream::defaultCommsType == Pstream::scheduled) {
    WARNING_IN
    (
      "processorCyclicFvPatchField<Type>::processorCyclicFvPatchField\n"
      "(\n"
      "    const fvPatch& p,\n"
      "    const DimensionedField<Type, volMesh>& iF,\n"
      "    const dictionary& dict\n"
      ")\n"
    )
    << "Scheduled communication with split cyclics not supported."
    << endl;
  }
}


template<class Type>
mousse::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
  const processorCyclicFvPatchField<Type>& ptf
)
:
  processorFvPatchField<Type>{ptf},
  procPatch_{refCast<const processorCyclicFvPatch>(ptf.patch())}
{}


template<class Type>
mousse::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
  const processorCyclicFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  processorFvPatchField<Type>{ptf, iF},
  procPatch_{refCast<const processorCyclicFvPatch>(ptf.patch())}
{}


// Destructor 
template<class Type>
mousse::processorCyclicFvPatchField<Type>::~processorCyclicFvPatchField()
{}

