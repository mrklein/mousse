// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "upstream.hpp"
#include "debug.hpp"
#include "register_switch.hpp"
#include "dictionary.hpp"
#include "iostreams.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(UPstream, 0);
template<>
const char* mousse::NamedEnum
<
  mousse::UPstream::commsTypes,
  3
>::names[] =
{
  "blocking",
  "scheduled",
  "nonBlocking"
};

}


const mousse::NamedEnum<mousse::UPstream::commsTypes, 3>
  mousse::UPstream::commsTypeNames;


// Private Member Functions 
void mousse::UPstream::setParRun(const label nProcs)
{
  parRun_ = true;
  // Redo worldComm communicator (this has been created at static
  // initialisation time)
  freeCommunicator(UPstream::worldComm);
  label comm = allocateCommunicator(-1, identity(nProcs), true);
  if (comm != UPstream::worldComm) {
    FATAL_ERROR_IN("UPstream::setParRun(const label)")
      << "problem : comm:" << comm
      << "  UPstream::worldComm:" << UPstream::worldComm
      << mousse::exit(FatalError);
  }
  Pout.prefix() = '[' +  name(myProcNo(Pstream::worldComm)) + "] ";
  Perr.prefix() = '[' +  name(myProcNo(Pstream::worldComm)) + "] ";
}


mousse::List<mousse::UPstream::commsStruct> mousse::UPstream::calcLinearComm
(
  const label nProcs
)
{
  List<commsStruct> linearCommunication{nProcs};
  // Master
  labelList belowIDs(nProcs - 1);
  FOR_ALL(belowIDs, i) {
    belowIDs[i] = i + 1;
  }
  linearCommunication[0] = commsStruct
  (
    nProcs,
    0,
    -1,
    belowIDs,
    labelList(0)
  );
  // Slaves. Have no below processors, only communicate up to master
  for (label procID = 1; procID < nProcs; procID++) {
    linearCommunication[procID] = commsStruct
    (
      nProcs,
      procID,
      0,
      labelList(0),
      labelList(0)
    );
  }
  return linearCommunication;
}


// Append my children (and my children children etc.) to allReceives.
void mousse::UPstream::collectReceives
(
  const label procID,
  const List<DynamicList<label> >& receives,
  DynamicList<label>& allReceives
)
{
  const DynamicList<label>& myChildren = receives[procID];
  FOR_ALL(myChildren, childI) {
    allReceives.append(myChildren[childI]);
    collectReceives(myChildren[childI], receives, allReceives);
  }
}
// Tree like schedule. For 8 procs:
// (level 0)
//      0 receives from 1
//      2 receives from 3
//      4 receives from 5
//      6 receives from 7
// (level 1)
//      0 receives from 2
//      4 receives from 6
// (level 2)
//      0 receives from 4
//
// The sends/receives for all levels are collected per processor (one send per
// processor; multiple receives possible) creating a table:
//
// So per processor:
// proc     receives from   sends to
// ----     -------------   --------
//  0       1,2,4           -
//  1       -               0
//  2       3               0
//  3       -               2
//  4       5               0
//  5       -               4
//  6       7               4
//  7       -               6
mousse::List<mousse::UPstream::commsStruct> mousse::UPstream::calcTreeComm
(
  label nProcs
)
{
  label nLevels = 1;
  while ((1 << nLevels) < nProcs) {
    nLevels++;
  }
  List<DynamicList<label>> receives{nProcs};
  labelList sends{nProcs, -1};
  // Info<< "Using " << nLevels << " communication levels" << endl;
  label offset = 2;
  label childOffset = offset/2;
  for (label level = 0; level < nLevels; level++) {
    label receiveID = 0;
    while (receiveID < nProcs) {
      // Determine processor that sends and we receive from
      label sendID = receiveID + childOffset;
      if (sendID < nProcs) {
        receives[receiveID].append(sendID);
        sends[sendID] = receiveID;
      }
      receiveID += offset;
    }
    offset <<= 1;
    childOffset <<= 1;
  }
  // For all processors find the processors it receives data from
  // (and the processors they receive data from etc.)
  List<DynamicList<label>> allReceives{nProcs};
  for (label procID = 0; procID < nProcs; procID++) {
    collectReceives(procID, receives, allReceives[procID]);
  }
  List<commsStruct> treeCommunication{nProcs};
  for (label procID = 0; procID < nProcs; procID++) {
    treeCommunication[procID] = commsStruct
    (
      nProcs,
      procID,
      sends[procID],
      receives[procID].shrink(),
      allReceives[procID].shrink()
    );
  }
  return treeCommunication;
}


mousse::label mousse::UPstream::allocateCommunicator
(
  const label parentIndex,
  const labelList& subRanks,
  const bool doPstream
)
{
  label index;
  if (!freeComms_.empty()) {
    index = freeComms_.pop();
  } else {
    // Extend storage
    index = parentCommunicator_.size();
    myProcNo_.append(-1);
    procIDs_.append(List<int>(0));
    parentCommunicator_.append(-1);
    linearCommunication_.append(List<commsStruct>(0));
    treeCommunication_.append(List<commsStruct>(0));
  }
  if (debug) {
    Pout << "Communicators : Allocating communicator " << index << endl
      << "    parent : " << parentIndex << endl
      << "    procs  : " << subRanks << endl
      << endl;
  }
  // Initialise; overwritten by allocatePstreamCommunicator
  myProcNo_[index] = 0;
  // Convert from label to int
  procIDs_[index].setSize(subRanks.size());
  FOR_ALL(procIDs_[index], i) {
    procIDs_[index][i] = subRanks[i];
    // Enforce incremental order (so index is rank in next communicator)
    if (i >= 1 && subRanks[i] <= subRanks[i-1]) {
      FATAL_ERROR_IN
      (
        "UPstream::allocateCommunicator"
        "(const label, const labelList&, const bool)"
      )
      << "subranks not sorted : " << subRanks
      << " when allocating subcommunicator from parent "
      << parentIndex
      << mousse::abort(FatalError);
    }
  }
  parentCommunicator_[index] = parentIndex;
  linearCommunication_[index] = calcLinearComm(procIDs_[index].size());
  treeCommunication_[index] = calcTreeComm(procIDs_[index].size());
  if (doPstream && parRun()) {
    allocatePstreamCommunicator(parentIndex, index);
  }
  return index;
}


void mousse::UPstream::freeCommunicator
(
  const label communicator,
  const bool doPstream
)
{
  if (debug) {
    Pout << "Communicators : Freeing communicator " << communicator << endl
      << "    parent   : " << parentCommunicator_[communicator] << endl
      << "    myProcNo : " << myProcNo_[communicator] << endl
      << endl;
  }
  if (doPstream && parRun()) {
    freePstreamCommunicator(communicator);
  }
  myProcNo_[communicator] = -1;
  //procIDs_[communicator].clear();
  parentCommunicator_[communicator] = -1;
  linearCommunication_[communicator].clear();
  treeCommunication_[communicator].clear();
  freeComms_.push(communicator);
}


void mousse::UPstream::freeCommunicators(const bool doPstream)
{
  FOR_ALL(myProcNo_, communicator) {
    if (myProcNo_[communicator] != -1) {
      freeCommunicator(communicator, doPstream);
    }
  }
}


int mousse::UPstream::baseProcNo(const label myComm, const int myProcID)
{
  int procID = myProcID;
  label comm = myComm;
  while (parent(comm) != -1) {
    const List<int>& parentRanks = UPstream::procID(comm);
    procID = parentRanks[procID];
    comm = UPstream::parent(comm);
  }
  return procID;
}


mousse::label mousse::UPstream::procNo(const label myComm, const int baseProcID)
{
  const List<int>& parentRanks = procID(myComm);
  label parentComm = parent(myComm);
  if (parentComm == -1) {
    return findIndex(parentRanks, baseProcID);
  } else {
    label parentRank = procNo(parentComm, baseProcID);
    return findIndex(parentRanks, parentRank);
  }
}


mousse::label mousse::UPstream::procNo
(
  const label myComm,
  const label currentComm,
  const int currentProcID
)
{
  label physProcID = UPstream::baseProcNo(currentComm, currentProcID);
  return procNo(myComm, physProcID);
}


// Static Data Members
// By default this is not a parallel run
bool mousse::UPstream::parRun_(false);

// Free communicators
mousse::LIFOStack<mousse::label> mousse::UPstream::freeComms_;

// My processor number
mousse::DynamicList<int> mousse::UPstream::myProcNo_{10};

// List of process IDs
mousse::DynamicList<mousse::List<int>> mousse::UPstream::procIDs_{10};

// Parent communicator
mousse::DynamicList<mousse::label> mousse::UPstream::parentCommunicator_{10};

// Standard transfer message type
int mousse::UPstream::msgType_{1};

// Linear communication schedule
mousse::DynamicList<mousse::List<mousse::UPstream::commsStruct>>
mousse::UPstream::linearCommunication_{10};

// Multi level communication schedule
mousse::DynamicList<mousse::List<mousse::UPstream::commsStruct>>
mousse::UPstream::treeCommunication_{10};

// Allocate a serial communicator. This gets overwritten in parallel mode
// (by UPstream::setParRun())
mousse::UPstream::communicator serialComm
(
  -1,
  mousse::labelList(1, mousse::label(0)),
  false
);

// Should compact transfer be used in which floats replace doubles
// reducing the bandwidth requirement at the expense of some loss
// in accuracy
bool mousse::UPstream::floatTransfer
(
  mousse::debug::optimisationSwitch("floatTransfer", 0)
);

REGISTER_OPT_SWITCH
(
  "floatTransfer",
  bool,
  mousse::UPstream::floatTransfer
);

// Number of processors at which the reduce algorithm changes from linear to
// tree
int mousse::UPstream::nProcsSimpleSum
(
  mousse::debug::optimisationSwitch("nProcsSimpleSum", 16)
);

REGISTER_OPT_SWITCH
(
  "nProcsSimpleSum",
  int,
  mousse::UPstream::nProcsSimpleSum
);

// Default commsType
mousse::UPstream::commsTypes mousse::UPstream::defaultCommsType
(
  commsTypeNames.read(mousse::debug::optimisationSwitches().lookup("commsType"))
);


namespace mousse {

// Register re-reader
class addcommsTypeToOpt
:
  public ::mousse::simpleRegIOobject
{
public:
  addcommsTypeToOpt(const char* name)
  :
    ::mousse::simpleRegIOobject(mousse::debug::addOptimisationObject, name)
  {}
  virtual ~addcommsTypeToOpt()
  {}
  virtual void readData(mousse::Istream& is)
  {
    UPstream::defaultCommsType = UPstream::commsTypeNames.read
    (
      is
    );
  }
  virtual void writeData(mousse::Ostream& os) const
  {
    os << UPstream::commsTypeNames[UPstream::defaultCommsType];
  }
};
addcommsTypeToOpt addcommsTypeToOpt_("commsType");

}

// Default communicator
mousse::label mousse::UPstream::worldComm{0};

// Warn for use of any communicator
mousse::label mousse::UPstream::warnComm{-1};

// Number of polling cycles in processor updates
int mousse::UPstream::nPollProcInterfaces
(
  mousse::debug::optimisationSwitch("nPollProcInterfaces", 0)
);


REGISTER_OPT_SWITCH
(
  "nPollProcInterfaces",
  int,
  mousse::UPstream::nPollProcInterfaces
);
