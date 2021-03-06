// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "upstream.hpp"
#include "pstream_reduce_ops.hpp"
#include "os_specific.hpp"
#include "pstream_globals.hpp"
#include "sub_list.hpp"
#include "all_reduce.hpp"
#include <mpi.h>
#include <cstring>
#include <cstdlib>
#include <csignal>
#if defined(WM_SP)
#   define MPI_SCALAR MPI_FLOAT
#elif defined(WM_DP)
#   define MPI_SCALAR MPI_DOUBLE
#endif


// NOTE:
// valid parallel options vary between implementations, but flag common ones.
// if they are not removed by MPI_Init(), the subsequent argument processing
// will notice that they are wrong
void mousse::UPstream::addValidParOptions(HashTable<string>& validParOptions)
{
  validParOptions.insert("np", "");
  validParOptions.insert("p4pg", "PI file");
  validParOptions.insert("p4wd", "directory");
  validParOptions.insert("p4amslave", "");
  validParOptions.insert("p4yourname", "hostname");
  validParOptions.insert("machinefile", "machine file");
}


bool mousse::UPstream::init(int& argc, char**& argv)
{
  MPI_Init(&argc, &argv);
  int numprocs;
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  int myRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  if (debug) {
    Pout << "UPstream::init : initialised with numProcs:" << numprocs
      << " myRank:" << myRank << endl;
  }
  if (numprocs <= 1) {
    FATAL_ERROR_IN("UPstream::init(int& argc, char**& argv)")
      << "bool IPstream::init(int& argc, char**& argv) : "
       "attempt to run parallel on 1 processor"
      << mousse::abort(FatalError);
  }
  // Initialise parallel structure
  setParRun(numprocs);
#ifndef SGIMPI
  string bufferSizeName = getEnv("MPI_BUFFER_SIZE");
  if (bufferSizeName.size()) {
    int bufferSize = atoi(bufferSizeName.c_str());
    if (bufferSize) {
      MPI_Buffer_attach(new char[bufferSize], bufferSize);
    }
  } else {
    FATAL_ERROR_IN("UPstream::init(int& argc, char**& argv)")
      << "UPstream::init(int& argc, char**& argv) : "
      << "environment variable MPI_BUFFER_SIZE not defined"
      << mousse::abort(FatalError);
  }
#endif
  return true;
}


void mousse::UPstream::exit(int errnum)
{
  if (debug) {
    Pout << "UPstream::exit." << endl;
  }
#ifndef SGIMPI
  int size;
  char* buff;
  MPI_Buffer_detach(&buff, &size);
  delete[] buff;
#endif
  if (PstreamGlobals::outstandingRequests_.size()) {
    label n = PstreamGlobals::outstandingRequests_.size();
    PstreamGlobals::outstandingRequests_.clear();
    WARNING_IN("UPstream::exit(int)")
      << "There are still " << n << " outstanding MPI_Requests." << endl
      << "This means that your code exited before doing a"
      << " UPstream::waitRequests()." << endl
      << "This should not happen for a normal code exit."
      << endl;
  }
  // Clean mpi communicators
  FOR_ALL(myProcNo_, communicator) {
    if (myProcNo_[communicator] != -1) {
      freePstreamCommunicator(communicator);
    }
  }
  if (errnum == 0) {
    MPI_Finalize();
    ::exit(errnum);
  } else {
    MPI_Abort(MPI_COMM_WORLD, errnum);
  }
}


void mousse::UPstream::abort()
{
  MPI_Abort(MPI_COMM_WORLD, 1);
}


void mousse::reduce
(
  scalar& Value,
  const sumOp<scalar>& bop,
  const int tag,
  const label communicator
)
{
  if (UPstream::warnComm != -1 && communicator != UPstream::warnComm) {
    Pout << "** reducing:" << Value << " with comm:" << communicator
      << " warnComm:" << UPstream::warnComm
      << endl;
    error::printStack(Pout);
  }
  allReduce(Value, 1, MPI_SCALAR, MPI_SUM, bop, tag, communicator);
}


void mousse::reduce
(
  scalar& Value,
  const minOp<scalar>& bop,
  const int tag,
  const label communicator
)
{
  if (UPstream::warnComm != -1 && communicator != UPstream::warnComm) {
    Pout << "** reducing:" << Value << " with comm:" << communicator
      << " warnComm:" << UPstream::warnComm
      << endl;
    error::printStack(Pout);
  }
  allReduce(Value, 1, MPI_SCALAR, MPI_MIN, bop, tag, communicator);
}


void mousse::reduce
(
  vector2D& Value,
  const sumOp<vector2D>& bop,
  const int tag,
  const label communicator
)
{
  if (UPstream::warnComm != -1 && communicator != UPstream::warnComm) {
    Pout << "** reducing:" << Value << " with comm:" << communicator
      << " warnComm:" << UPstream::warnComm
      << endl;
    error::printStack(Pout);
  }
  allReduce(Value, 2, MPI_SCALAR, MPI_SUM, bop, tag, communicator);
}


void mousse::sumReduce
(
  scalar& Value,
  label& Count,
  const int tag,
  const label communicator
)
{
  if (UPstream::warnComm != -1 && communicator != UPstream::warnComm) {
    Pout << "** reducing:" << Value << " with comm:" << communicator
      << " warnComm:" << UPstream::warnComm
      << endl;
    error::printStack(Pout);
  }
  vector2D twoScalars(Value, scalar(Count));
  reduce(twoScalars, sumOp<vector2D>(), tag, communicator);
  Value = twoScalars.x();
  Count = twoScalars.y();
}


void mousse::reduce
(
  scalar& Value,
  const sumOp<scalar>& bop,
  const int tag,
  const label communicator,
  label& requestID
)
{
#ifdef MPIX_COMM_TYPE_SHARED
  // Assume mpich2 with non-blocking collectives extensions. Once mpi3
  // is available this will change.
  MPI_Request request;
  scalar v = Value;
  MPIX_Ireduce
  (
    &v,
    &Value,
    1,
    MPI_SCALAR,
    MPI_SUM,
    0,              //root
    PstreamGlobals::MPICommunicators_[communicator],
    &request
  );
  requestID = PstreamGlobals::outstandingRequests_.size();
  PstreamGlobals::outstandingRequests_.append(request);
  if (UPstream::debug) {
    Pout << "UPstream::allocateRequest for non-blocking reduce"
      << " : request:" << requestID
      << endl;
  }
#else
  // Non-blocking not yet implemented in mpi
  reduce(Value, bop, tag, communicator);
  requestID = -1;
#endif
}


void mousse::UPstream::allocatePstreamCommunicator
(
  const label parentIndex,
  const label index
)
{
  if (index == PstreamGlobals::MPIGroups_.size()) {
    // Extend storage with dummy values
    MPI_Group newGroup = MPI_GROUP_NULL;
    PstreamGlobals::MPIGroups_.append(newGroup);
    MPI_Comm newComm = MPI_COMM_NULL;
    PstreamGlobals::MPICommunicators_.append(newComm);
  } else if (index > PstreamGlobals::MPIGroups_.size()) {
    FATAL_ERROR_IN
    (
      "UPstream::allocatePstreamCommunicator\n"
      "(\n"
      "    const label parentIndex,\n"
      "    const labelList& subRanks\n"
      ")\n"
    )
    << "PstreamGlobals out of sync with UPstream data. Problem."
    << mousse::exit(FatalError);
  }
  if (parentIndex == -1) {
    // Allocate world communicator
    if (index != UPstream::worldComm) {
      FATAL_ERROR_IN
      (
        "UPstream::allocateCommunicator\n"
        "(\n"
        "    const label parentIndex,\n"
        "    const labelList& subRanks\n"
        ")\n"
      )
      << "world communicator should always be index "
      << UPstream::worldComm << mousse::exit(FatalError);
    }
    PstreamGlobals::MPICommunicators_[index] = MPI_COMM_WORLD;
    MPI_Comm_group(MPI_COMM_WORLD, &PstreamGlobals::MPIGroups_[index]);
    MPI_Comm_rank
    (
      PstreamGlobals::MPICommunicators_[index],
      &myProcNo_[index]
    );
    // Set the number of processes to the actual number
    int numProcs;
    MPI_Comm_size(PstreamGlobals::MPICommunicators_[index], &numProcs);
    //procIDs_[index] = identity(numProcs);
    procIDs_[index].setSize(numProcs);
    FOR_ALL(procIDs_[index], i) {
      procIDs_[index][i] = i;
    }
  } else {
    // Create new group
    MPI_Group_incl
    (
      PstreamGlobals::MPIGroups_[parentIndex],
      procIDs_[index].size(),
      procIDs_[index].begin(),
      &PstreamGlobals::MPIGroups_[index]
    );
    // Create new communicator
    MPI_Comm_create
    (
      PstreamGlobals::MPICommunicators_[parentIndex],
      PstreamGlobals::MPIGroups_[index],
      &PstreamGlobals::MPICommunicators_[index]
    );
    if (PstreamGlobals::MPICommunicators_[index] == MPI_COMM_NULL) {
      myProcNo_[index] = -1;
    } else {
      if
      (
        MPI_Comm_rank
        (
          PstreamGlobals::MPICommunicators_[index],
          &myProcNo_[index]
        )
      ) {
        FATAL_ERROR_IN
        (
          "UPstream::allocatePstreamCommunicator\n"
          "(\n"
          "    const label,\n"
          "    const labelList&\n"
          ")\n"
        )
        << "Problem :"
        << " when allocating communicator at " << index
        << " from ranks " << procIDs_[index]
        << " of parent " << parentIndex
        << " cannot find my own rank"
        << mousse::exit(FatalError);
      }
    }
  }
}


void mousse::UPstream::freePstreamCommunicator(const label communicator)
{
  if (communicator != UPstream::worldComm) {
    if (PstreamGlobals::MPICommunicators_[communicator] != MPI_COMM_NULL) {
      // Free communicator. Sets communicator to MPI_COMM_NULL
      MPI_Comm_free(&PstreamGlobals::MPICommunicators_[communicator]);
    }
    if (PstreamGlobals::MPIGroups_[communicator] != MPI_GROUP_NULL) {
      // Free greoup. Sets group to MPI_GROUP_NULL
      MPI_Group_free(&PstreamGlobals::MPIGroups_[communicator]);
    }
  }
}


mousse::label mousse::UPstream::nRequests()
{
  return PstreamGlobals::outstandingRequests_.size();
}


void mousse::UPstream::resetRequests(const label i)
{
  if (i < PstreamGlobals::outstandingRequests_.size()) {
    PstreamGlobals::outstandingRequests_.setSize(i);
  }
}


void mousse::UPstream::waitRequests(const label start)
{
  if (debug) {
    Pout << "UPstream::waitRequests : starting wait for "
      << PstreamGlobals::outstandingRequests_.size()-start
      << " outstanding requests starting at " << start << endl;
  }
  if (PstreamGlobals::outstandingRequests_.size()) {
    SubList<MPI_Request> waitRequests
    (
      PstreamGlobals::outstandingRequests_,
      PstreamGlobals::outstandingRequests_.size() - start,
      start
    );
    if
    (
      MPI_Waitall
      (
        waitRequests.size(),
        waitRequests.begin(),
        MPI_STATUSES_IGNORE
      )
    ) {
      FATAL_ERROR_IN
      (
        "UPstream::waitRequests()"
      )
      << "MPI_Waitall returned with error" << mousse::endl;
    }
    resetRequests(start);
  }
  if (debug) {
    Pout << "UPstream::waitRequests : finished wait." << endl;
  }
}


void mousse::UPstream::waitRequest(const label i)
{
  if (debug) {
    Pout << "UPstream::waitRequest : starting wait for request:" << i
      << endl;
  }
  if (i >= PstreamGlobals::outstandingRequests_.size()) {
    FATAL_ERROR_IN
    (
      "UPstream::waitRequest(const label)"
    )
    << "There are " << PstreamGlobals::outstandingRequests_.size()
    << " outstanding send requests and you are asking for i=" << i
    << nl
    << "Maybe you are mixing blocking/non-blocking comms?"
    << mousse::abort(FatalError);
  }
  if
  (
    MPI_Wait
    (
     &PstreamGlobals::outstandingRequests_[i],
      MPI_STATUS_IGNORE
    )
  ) {
    FATAL_ERROR_IN
    (
      "UPstream::waitRequest()"
    )
    << "MPI_Wait returned with error" << mousse::endl;
  }
  if (debug) {
    Pout << "UPstream::waitRequest : finished wait for request:" << i
      << endl;
  }
}


bool mousse::UPstream::finishedRequest(const label i)
{
  if (debug) {
    Pout << "UPstream::finishedRequest : checking request:" << i
      << endl;
  }
  if (i >= PstreamGlobals::outstandingRequests_.size()) {
    FATAL_ERROR_IN
    (
      "UPstream::finishedRequest(const label)"
    )
    << "There are " << PstreamGlobals::outstandingRequests_.size()
    << " outstanding send requests and you are asking for i=" << i
    << nl
    << "Maybe you are mixing blocking/non-blocking comms?"
    << mousse::abort(FatalError);
  }
  int flag;
  MPI_Test
  (
    &PstreamGlobals::outstandingRequests_[i],
    &flag,
    MPI_STATUS_IGNORE
  );
  if (debug) {
    Pout << "UPstream::finishedRequest : finished request:" << i
      << endl;
  }
  return flag != 0;
}


int mousse::UPstream::allocateTag(const char* s)
{
  int tag;
  if (PstreamGlobals::freedTags_.size()) {
    tag = PstreamGlobals::freedTags_.remove();
  } else {
    tag = PstreamGlobals::nTags_++;
  }
  if (debug) {
    Pout << "UPstream::allocateTag " << s
      << " : tag:" << tag
      << endl;
  }
  return tag;
}


int mousse::UPstream::allocateTag(const word& s)
{
  int tag;
  if (PstreamGlobals::freedTags_.size()) {
    tag = PstreamGlobals::freedTags_.remove();
  } else {
    tag = PstreamGlobals::nTags_++;
  }
  if (debug) {
    Pout << "UPstream::allocateTag " << s
      << " : tag:" << tag
      << endl;
  }
  return tag;
}


void mousse::UPstream::freeTag(const char* s, const int tag)
{
  if (debug) {
    Pout << "UPstream::freeTag " << s << " tag:" << tag << endl;
  }
  PstreamGlobals::freedTags_.append(tag);
}


void mousse::UPstream::freeTag(const word& s, const int tag)
{
  if (debug) {
    Pout << "UPstream::freeTag " << s << " tag:" << tag << endl;
  }
  PstreamGlobals::freedTags_.append(tag);
}

