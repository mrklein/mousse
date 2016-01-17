// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_gamg_interface_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ldu_matrix.hpp"

// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(processorGAMGInterfaceField, 0);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  processorGAMGInterfaceField,
  lduInterface
);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  processorGAMGInterfaceField,
  lduInterfaceField
);

}

// Constructors
mousse::processorGAMGInterfaceField::processorGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const lduInterfaceField& fineInterface
)
:
  GAMGInterfaceField(GAMGCp, fineInterface),
  procInterface_(refCast<const processorGAMGInterface>(GAMGCp)),
  doTransform_(false),
  rank_(0)
{
  const processorLduInterfaceField& p =
    refCast<const processorLduInterfaceField>(fineInterface);
  doTransform_ = p.doTransform();
  rank_ = p.rank();
}
mousse::processorGAMGInterfaceField::processorGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const bool doTransform,
  const int rank
)
:
  GAMGInterfaceField(GAMGCp, doTransform, rank),
  procInterface_(refCast<const processorGAMGInterface>(GAMGCp)),
  doTransform_(doTransform),
  rank_(rank)
{}

// Destructor
mousse::processorGAMGInterfaceField::~processorGAMGInterfaceField()
{}

// Member Functions
void mousse::processorGAMGInterfaceField::initInterfaceMatrixUpdate
(
  scalarField&,
  const scalarField& psiInternal,
  const scalarField&,
  const direction,
  const Pstream::commsTypes commsType
) const
{
  label oldWarn = UPstream::warnComm;
  UPstream::warnComm = comm();
  procInterface_.interfaceInternalField(psiInternal, scalarSendBuf_);
  if (commsType == Pstream::nonBlocking && !Pstream::floatTransfer)
  {
    // Fast path.
    scalarReceiveBuf_.setSize(scalarSendBuf_.size());
    outstandingRecvRequest_ = UPstream::nRequests();
    IPstream::read
    (
      Pstream::nonBlocking,
      procInterface_.neighbProcNo(),
      reinterpret_cast<char*>(scalarReceiveBuf_.begin()),
      scalarReceiveBuf_.byteSize(),
      procInterface_.tag(),
      comm()
    );
    outstandingSendRequest_ = UPstream::nRequests();
    OPstream::write
    (
      Pstream::nonBlocking,
      procInterface_.neighbProcNo(),
      reinterpret_cast<const char*>(scalarSendBuf_.begin()),
      scalarSendBuf_.byteSize(),
      procInterface_.tag(),
      comm()
    );
  }
  else
  {
    procInterface_.compressedSend(commsType, scalarSendBuf_);
  }
  const_cast<processorGAMGInterfaceField&>(*this).updatedMatrix() = false;
  UPstream::warnComm = oldWarn;
}
void mousse::processorGAMGInterfaceField::updateInterfaceMatrix
(
  scalarField& result,
  const scalarField&,
  const scalarField& coeffs,
  const direction cmpt,
  const Pstream::commsTypes commsType
) const
{
  if (updatedMatrix())
  {
    return;
  }
  label oldWarn = UPstream::warnComm;
  UPstream::warnComm = comm();
  const labelUList& faceCells = procInterface_.faceCells();
  if (commsType == Pstream::nonBlocking && !Pstream::floatTransfer)
  {
    // Fast path.
    if
    (
      outstandingRecvRequest_ >= 0
    && outstandingRecvRequest_ < Pstream::nRequests()
    )
    {
      UPstream::waitRequest(outstandingRecvRequest_);
    }
    // Recv finished so assume sending finished as well.
    outstandingSendRequest_ = -1;
    outstandingRecvRequest_ = -1;
    // Consume straight from scalarReceiveBuf_
    // Transform according to the transformation tensor
    transformCoupleField(scalarReceiveBuf_, cmpt);
    // Multiply the field by coefficients and add into the result
    FOR_ALL(faceCells, elemI)
    {
      result[faceCells[elemI]] -= coeffs[elemI]*scalarReceiveBuf_[elemI];
    }
  }
  else
  {
    scalarField pnf
    (
      procInterface_.compressedReceive<scalar>(commsType, coeffs.size())
    );
    transformCoupleField(pnf, cmpt);
    FOR_ALL(faceCells, elemI)
    {
      result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
    }
  }
  const_cast<processorGAMGInterfaceField&>(*this).updatedMatrix() = true;
  UPstream::warnComm = oldWarn;
}
