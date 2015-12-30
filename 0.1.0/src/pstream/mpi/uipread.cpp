// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uipstream.hpp"
#include "pstream_globals.hpp"
#include "iostreams.hpp"
#include <mpi.h>
// Constructor
mousse::UIPstream::UIPstream
(
  const commsTypes commsType,
  const int fromProcNo,
  DynamicList<char>& externalBuf,
  label& externalBufPosition,
  const int tag,
  const label comm,
  const bool clearAtEnd,
  streamFormat format,
  versionNumber version
)
:
  UPstream(commsType),
  Istream(format, version),
  fromProcNo_(fromProcNo),
  externalBuf_(externalBuf),
  externalBufPosition_(externalBufPosition),
  tag_(tag),
  comm_(comm),
  clearAtEnd_(clearAtEnd),
  messageSize_(0)
{
  setOpened();
  setGood();
  if (commsType == UPstream::nonBlocking)
  {
    // Message is already received into externalBuf
  }
  else
  {
    MPI_Status status;
    label wantedSize = externalBuf_.capacity();
    if (debug)
    {
      Pout<< "UIPstream::UIPstream : read from:" << fromProcNo
        << " tag:" << tag << " comm:" << comm_
        << " wanted size:" << wantedSize
        << mousse::endl;
    }
    // If the buffer size is not specified, probe the incomming message
    // and set it
    if (!wantedSize)
    {
      MPI_Probe
      (
        fromProcNo_,
        tag_,
        PstreamGlobals::MPICommunicators_[comm_],
        &status
      );
      MPI_Get_count(&status, MPI_BYTE, &messageSize_);
      externalBuf_.setCapacity(messageSize_);
      wantedSize = messageSize_;
      if (debug)
      {
        Pout<< "UIPstream::UIPstream : probed size:" << wantedSize
          << mousse::endl;
      }
    }
    messageSize_ = UIPstream::read
    (
      commsType,
      fromProcNo_,
      externalBuf_.begin(),
      wantedSize,
      tag_,
      comm_
    );
    // Set addressed size. Leave actual allocated memory intact.
    externalBuf_.setSize(messageSize_);
    if (!messageSize_)
    {
      setEof();
    }
  }
}
mousse::UIPstream::UIPstream(const int fromProcNo, PstreamBuffers& buffers)
:
  UPstream(buffers.commsType_),
  Istream(buffers.format_, buffers.version_),
  fromProcNo_(fromProcNo),
  externalBuf_(buffers.recvBuf_[fromProcNo]),
  externalBufPosition_(buffers.recvBufPos_[fromProcNo]),
  tag_(buffers.tag_),
  comm_(buffers.comm_),
  clearAtEnd_(true),
  messageSize_(0)
{
  if (commsType() != UPstream::scheduled && !buffers.finishedSendsCalled_)
  {
    FatalErrorIn("UIPstream::UIPstream(const int, PstreamBuffers&)")
      << "PstreamBuffers::finishedSends() never called." << endl
      << "Please call PstreamBuffers::finishedSends() after doing"
      << " all your sends (using UOPstream) and before doing any"
      << " receives (using UIPstream)" << mousse::exit(FatalError);
  }
  setOpened();
  setGood();
  if (commsType() == UPstream::nonBlocking)
  {
    // Message is already received into externalBuf
    messageSize_ = buffers.recvBuf_[fromProcNo].size();
    if (debug)
    {
      Pout<< "UIPstream::UIPstream PstreamBuffers :"
        << " fromProcNo:" << fromProcNo
        << " tag:" << tag_ << " comm:" << comm_
        << " receive buffer size:" << messageSize_
        << mousse::endl;
    }
  }
  else
  {
    MPI_Status status;
    label wantedSize = externalBuf_.capacity();
    if (debug)
    {
      Pout<< "UIPstream::UIPstream PstreamBuffers :"
        << " read from:" << fromProcNo
        << " tag:" << tag_ << " comm:" << comm_
        << " wanted size:" << wantedSize
        << mousse::endl;
    }
    // If the buffer size is not specified, probe the incomming message
    // and set it
    if (!wantedSize)
    {
      MPI_Probe
      (
        fromProcNo_,
        tag_,
        PstreamGlobals::MPICommunicators_[comm_],
        &status
      );
      MPI_Get_count(&status, MPI_BYTE, &messageSize_);
      externalBuf_.setCapacity(messageSize_);
      wantedSize = messageSize_;
      if (debug)
      {
        Pout<< "UIPstream::UIPstream PstreamBuffers : probed size:"
          << wantedSize << mousse::endl;
      }
    }
    messageSize_ = UIPstream::read
    (
      commsType(),
      fromProcNo_,
      externalBuf_.begin(),
      wantedSize,
      tag_,
      comm_
    );
    // Set addressed size. Leave actual allocated memory intact.
    externalBuf_.setSize(messageSize_);
    if (!messageSize_)
    {
      setEof();
    }
  }
}
// Member Functions 
mousse::label mousse::UIPstream::read
(
  const commsTypes commsType,
  const int fromProcNo,
  char* buf,
  const std::streamsize bufSize,
  const int tag,
  const label communicator
)
{
  if (debug)
  {
    Pout<< "UIPstream::read : starting read from:" << fromProcNo
      << " tag:" << tag << " comm:" << communicator
      << " wanted size:" << label(bufSize)
      << " commsType:" << UPstream::commsTypeNames[commsType]
      << mousse::endl;
  }
  if (UPstream::warnComm != -1 && communicator != UPstream::warnComm)
  {
    Pout<< "UIPstream::read : starting read from:" << fromProcNo
      << " tag:" << tag << " comm:" << communicator
      << " wanted size:" << label(bufSize)
      << " commsType:" << UPstream::commsTypeNames[commsType]
      << " warnComm:" << UPstream::warnComm
      << mousse::endl;
    error::printStack(Pout);
  }
  if (commsType == blocking || commsType == scheduled)
  {
    MPI_Status status;
    if
    (
      MPI_Recv
      (
        buf,
        bufSize,
        MPI_BYTE,
        fromProcNo,
        tag,
        PstreamGlobals::MPICommunicators_[communicator],
        &status
      )
    )
    {
      FatalErrorIn
      (
        "UIPstream::read"
        "(const int fromProcNo, char* buf, std::streamsize bufSize)"
      )   << "MPI_Recv cannot receive incomming message"
        << mousse::abort(FatalError);
      return 0;
    }
    // Check size of message read
    int messageSize;
    MPI_Get_count(&status, MPI_BYTE, &messageSize);
    if (debug)
    {
      Pout<< "UIPstream::read : finished read from:" << fromProcNo
        << " tag:" << tag << " read size:" << label(bufSize)
        << " commsType:" << UPstream::commsTypeNames[commsType]
        << mousse::endl;
    }
    if (messageSize > bufSize)
    {
      FatalErrorIn
      (
        "UIPstream::read"
        "(const int fromProcNo, char* buf, std::streamsize bufSize)"
      )   << "buffer (" << label(bufSize)
        << ") not large enough for incomming message ("
        << messageSize << ')'
        << mousse::abort(FatalError);
    }
    return messageSize;
  }
  else if (commsType == nonBlocking)
  {
    MPI_Request request;
    if
    (
      MPI_Irecv
      (
        buf,
        bufSize,
        MPI_BYTE,
        fromProcNo,
        tag,
        PstreamGlobals::MPICommunicators_[communicator],
        &request
      )
    )
    {
      FatalErrorIn
      (
        "UIPstream::read"
        "(const int fromProcNo, char* buf, std::streamsize bufSize)"
      )   << "MPI_Recv cannot start non-blocking receive"
        << mousse::abort(FatalError);
      return 0;
    }
    if (debug)
    {
      Pout<< "UIPstream::read : started read from:" << fromProcNo
        << " tag:" << tag << " read size:" << label(bufSize)
        << " commsType:" << UPstream::commsTypeNames[commsType]
        << " request:" << PstreamGlobals::outstandingRequests_.size()
        << mousse::endl;
    }
    PstreamGlobals::outstandingRequests_.append(request);
    // Assume the message is completely received.
    return bufSize;
  }
  else
  {
    FatalErrorIn
    (
      "UIPstream::read"
      "(const int fromProcNo, char* buf, std::streamsize bufSize)"
    )   << "Unsupported communications type "
      << commsType
      << mousse::abort(FatalError);
    return 0;
  }
}
