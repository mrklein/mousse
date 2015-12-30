// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uopstream.hpp"
#include "pstream_globals.hpp"
#include <mpi.h>
// Member Functions 
bool mousse::UOPstream::write
(
  const commsTypes commsType,
  const int toProcNo,
  const char* buf,
  const std::streamsize bufSize,
  const int tag,
  const label communicator
)
{
  if (debug)
  {
    Pout<< "UOPstream::write : starting write to:" << toProcNo
      << " tag:" << tag
      << " comm:" << communicator << " size:" << label(bufSize)
      << " commsType:" << UPstream::commsTypeNames[commsType]
      << mousse::endl;
  }
  if (UPstream::warnComm != -1 && communicator != UPstream::warnComm)
  {
    Pout<< "UOPstream::write : starting write to:" << toProcNo
      << " tag:" << tag
      << " comm:" << communicator << " size:" << label(bufSize)
      << " commsType:" << UPstream::commsTypeNames[commsType]
      << " warnComm:" << UPstream::warnComm
      << mousse::endl;
    error::printStack(Pout);
  }
  PstreamGlobals::checkCommunicator(communicator, toProcNo);
  bool transferFailed = true;
  if (commsType == blocking)
  {
    transferFailed = MPI_Bsend
    (
      const_cast<char*>(buf),
      bufSize,
      MPI_BYTE,
      toProcNo,   //procID(toProcNo),
      tag,
      PstreamGlobals::MPICommunicators_[communicator] //MPI_COMM_WORLD
    );
    if (debug)
    {
      Pout<< "UOPstream::write : finished write to:" << toProcNo
        << " tag:" << tag << " size:" << label(bufSize)
        << " commsType:" << UPstream::commsTypeNames[commsType]
        << mousse::endl;
    }
  }
  else if (commsType == scheduled)
  {
    transferFailed = MPI_Send
    (
      const_cast<char*>(buf),
      bufSize,
      MPI_BYTE,
      toProcNo,   //procID(toProcNo),
      tag,
      PstreamGlobals::MPICommunicators_[communicator] //MPI_COMM_WORLD
    );
    if (debug)
    {
      Pout<< "UOPstream::write : finished write to:" << toProcNo
        << " tag:" << tag << " size:" << label(bufSize)
        << " commsType:" << UPstream::commsTypeNames[commsType]
        << mousse::endl;
    }
  }
  else if (commsType == nonBlocking)
  {
    MPI_Request request;
    transferFailed = MPI_Isend
    (
      const_cast<char*>(buf),
      bufSize,
      MPI_BYTE,
      toProcNo,   //procID(toProcNo),
      tag,
      PstreamGlobals::MPICommunicators_[communicator],//MPI_COMM_WORLD,
      &request
    );
    if (debug)
    {
      Pout<< "UOPstream::write : started write to:" << toProcNo
        << " tag:" << tag << " size:" << label(bufSize)
        << " commsType:" << UPstream::commsTypeNames[commsType]
        << " request:" << PstreamGlobals::outstandingRequests_.size()
        << mousse::endl;
    }
    PstreamGlobals::outstandingRequests_.append(request);
  }
  else
  {
    FatalErrorIn
    (
      "UOPstream::write"
      "(const int fromProcNo, char* buf, std::streamsize bufSize"
      ", const int)"
    )   << "Unsupported communications type "
      << UPstream::commsTypeNames[commsType]
      << mousse::abort(FatalError);
  }
  return !transferFailed;
}
