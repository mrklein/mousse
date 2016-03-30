// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pstream_buffers.hpp"


namespace mousse {

DynamicList<char> PstreamBuffers::nullBuf(0);

}


// Constructor
mousse::PstreamBuffers::PstreamBuffers
(
  const UPstream::commsTypes commsType,
  const int tag,
  const label comm,
  IOstream::streamFormat format,
  IOstream::versionNumber version
)
:
  commsType_{commsType},
  tag_{tag},
  comm_{comm},
  format_{format},
  version_{version},
  sendBuf_{UPstream::nProcs(comm)},
  recvBuf_{UPstream::nProcs(comm)},
  recvBufPos_{UPstream::nProcs(comm),  0},
  finishedSendsCalled_{false}
{}


// Destructor 
mousse::PstreamBuffers::~PstreamBuffers()
{
  // Check that all data has been consumed.
  FOR_ALL(recvBufPos_, procI) {
    if (recvBufPos_[procI] < recvBuf_[procI].size()) {
      FATAL_ERROR_IN("PstreamBuffers::~PstreamBuffers()")
        << "Message from processor " << procI
        << " not fully consumed. messageSize:" << recvBuf_[procI].size()
        << " bytes of which only " << recvBufPos_[procI]
        << " consumed."
        << mousse::abort(FatalError);
    }
  }
}


// Member Functions 
void mousse::PstreamBuffers::finishedSends(const bool block)
{
  finishedSendsCalled_ = true;
  if (commsType_ == UPstream::nonBlocking) {
    labelListList sizes;
    Pstream::exchange<DynamicList<char>, char>
    (
      sendBuf_,
      recvBuf_,
      sizes,
      tag_,
      comm_,
      block
    );
  }
}


void mousse::PstreamBuffers::finishedSends(labelListList& sizes, const bool block)
{
  finishedSendsCalled_ = true;
  if (commsType_ == UPstream::nonBlocking) {
    Pstream::exchange<DynamicList<char>, char>
    (
      sendBuf_,
      recvBuf_,
      sizes,
      tag_,
      comm_,
      block
    );
  } else {
    FATAL_ERROR_IN
    (
      "PstreamBuffers::finishedSends(labelListList&, const bool)"
    )
    << "Obtaining sizes not supported in "
    << UPstream::commsTypeNames[commsType_] << endl
    << " since transfers already in progress. Use non-blocking instead."
    << exit(FatalError);
    // Note: possible only if using different tag from write started
    // by ~UOPstream. Needs some work.
    //sizes.setSize(UPstream::nProcs(comm));
    //labelList& nsTransPs = sizes[UPstream::myProcNo(comm)];
    //nsTransPs.setSize(UPstream::nProcs(comm));
    //
    //FOR_ALL(sendBuf_, procI)
    //{
    //    nsTransPs[procI] = sendBuf_[procI].size();
    //}
    //
    //// Send sizes across.
    //int oldTag = UPstream::msgType();
    //UPstream::msgType() = tag_;
    //combineReduce(sizes, UPstream::listEq());
    //UPstream::msgType() = oldTag;
  }
}


void mousse::PstreamBuffers::clear()
{
  FOR_ALL(sendBuf_, i) {
    sendBuf_[i].clear();
  }
  FOR_ALL(recvBuf_, i) {
    recvBuf_[i].clear();
  }
  recvBufPos_ = 0;
  finishedSendsCalled_ = false;
}
