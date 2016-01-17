// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_cyclic_point_patch_field.hpp"
#include "transform_field.hpp"
#include "processor_poly_patch.hpp"
// Constructors
template<class Type>
mousse::processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  coupledPointPatchField<Type>(p, iF),
  procPatch_(refCast<const processorCyclicPointPatch>(p)),
  receiveBuf_(0)
{}
template<class Type>
mousse::processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  coupledPointPatchField<Type>(p, iF, dict),
  procPatch_(refCast<const processorCyclicPointPatch>(p)),
  receiveBuf_(0)
{}
template<class Type>
mousse::processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
  const processorCyclicPointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  coupledPointPatchField<Type>(ptf, p, iF, mapper),
  procPatch_(refCast<const processorCyclicPointPatch>(ptf.patch())),
  receiveBuf_(0)
{}
template<class Type>
mousse::processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
  const processorCyclicPointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  coupledPointPatchField<Type>(ptf, iF),
  procPatch_(refCast<const processorCyclicPointPatch>(ptf.patch())),
  receiveBuf_(0)
{}
// Destructor 
template<class Type>
mousse::processorCyclicPointPatchField<Type>::~processorCyclicPointPatchField()
{}
// Member Functions 
template<class Type>
void mousse::processorCyclicPointPatchField<Type>::initSwapAddSeparated
(
  const Pstream::commsTypes commsType,
  Field<Type>& pField
) const
{
  if (Pstream::parRun())
  {
    // Get internal field into correct order for opposite side
    Field<Type> pf
    (
      this->patchInternalField
      (
        pField,
        procPatch_.reverseMeshPoints()
      )
    );
    if (commsType == Pstream::nonBlocking)
    {
      receiveBuf_.setSize(pf.size());
      IPstream::read
      (
        commsType,
        procPatch_.neighbProcNo(),
        reinterpret_cast<char*>(receiveBuf_.begin()),
        receiveBuf_.byteSize(),
        procPatch_.tag(),
        procPatch_.comm()
      );
    }
    OPstream::write
    (
      commsType,
      procPatch_.neighbProcNo(),
      reinterpret_cast<const char*>(pf.begin()),
      pf.byteSize(),
      procPatch_.tag(),
      procPatch_.comm()
    );
  }
}
template<class Type>
void mousse::processorCyclicPointPatchField<Type>::swapAddSeparated
(
  const Pstream::commsTypes commsType,
  Field<Type>& pField
) const
{
  if (Pstream::parRun())
  {
    // If nonblocking data has already been received into receiveBuf_
    if (commsType != Pstream::nonBlocking)
    {
      receiveBuf_.setSize(this->size());
      IPstream::read
      (
        commsType,
        procPatch_.neighbProcNo(),
        reinterpret_cast<char*>(receiveBuf_.begin()),
        receiveBuf_.byteSize(),
        procPatch_.tag(),
        procPatch_.comm()
      );
    }
    if (doTransform())
    {
      const processorCyclicPolyPatch& ppp =
        procPatch_.procCyclicPolyPatch();
      const tensor& forwardT = ppp.forwardT()[0];
      transform(receiveBuf_, forwardT, receiveBuf_);
    }
    // All points are separated
    this->addToInternalField(pField, receiveBuf_);
  }
}
