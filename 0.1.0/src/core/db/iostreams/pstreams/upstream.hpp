// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UPstream
// Description
//   Inter-processor communications stream
// SourceFiles
//   upstream.cpp
//   upstream_comms_struct.cpp
//   gather_scatter.cpp
//   combine_gather_scatter.cpp
//   gather_scatter_list.cpp
#ifndef upstream_hpp_
#define upstream_hpp_
#include "label_list.hpp"
#include "dynamic_list.hpp"
#include "hash_table.hpp"
#include "string.hpp"
#include "named_enum.hpp"
#include "list_ops.hpp"
#include "lifo_stack.hpp"
namespace mousse
{
class UPstream
{
public:
  //- Types of communications
  enum commsTypes
  {
    blocking,
    scheduled,
    nonBlocking
  };
  static const NamedEnum<commsTypes, 3> commsTypeNames;
  // Public classes
    //- Structure for communicating between processors
    class commsStruct
    {
      // Private data
        //- procID of above processor
        label above_;
        //- procIDs of processors directly below me
        labelList below_;
        //- procIDs of all processors below (so not just directly below)
        labelList allBelow_;
        //- procIDs of all processors not below. (inverse set of
        //  allBelow_ and minus myProcNo)
        labelList allNotBelow_;
    public:
      // Constructors
        //- Construct null
        commsStruct();
        //- Construct from components
        commsStruct
        (
          const label,
          const labelList&,
          const labelList&,
          const labelList&
        );
        //- Construct from components; construct allNotBelow_
        commsStruct
        (
          const label nProcs,
          const label myProcID,
          const label,
          const labelList&,
          const labelList&
        );
      // Member Functions
        // Access
          label above() const
          {
            return above_;
          }
          const labelList& below() const
          {
            return below_;
          }
          const labelList& allBelow() const
          {
            return allBelow_;
          }
          const labelList& allNotBelow() const
          {
            return allNotBelow_;
          }
      // Member operators
        bool operator==(const commsStruct&) const;
        bool operator!=(const commsStruct&) const;
      // Ostream Operator
        friend Ostream& operator<<(Ostream&, const commsStruct&);
    };
    //- combineReduce operator for lists. Used for counting.
    class listEq
    {
    public:
      template<class T>
      void operator()(T& x, const T& y) const
      {
        FOR_ALL(y, i)
        {
          if (y[i].size())
          {
            x[i] = y[i];
          }
        }
      }
    };
private:
  // Private data
    static bool parRun_;
    static int msgType_;
    // Communicator specific data
    static LIFOStack<label> freeComms_;
    static DynamicList<int> myProcNo_;
    static DynamicList<List<int> > procIDs_;
    static DynamicList<label> parentCommunicator_;
    static DynamicList<List<commsStruct> > linearCommunication_;
    static DynamicList<List<commsStruct> > treeCommunication_;
  // Private Member Functions
    //- Set data for parallel running
    static void setParRun(const label nProcs);
    //- Calculate linear communication schedule
    static List<commsStruct> calcLinearComm(const label nProcs);
    //- Calculate tree communication schedule
    static List<commsStruct> calcTreeComm(const label nProcs);
    //- Helper function for tree communication schedule determination
    //  Collects all processorIDs below a processor
    static void collectReceives
    (
      const label procID,
      const List<DynamicList<label> >& receives,
      DynamicList<label>& allReceives
    );
    //- Allocate a communicator with index
    static void allocatePstreamCommunicator
    (
      const label parentIndex,
      const label index
    );
    //- Free a communicator
    static void freePstreamCommunicator
    (
      const label index
    );
protected:
  // Protected data
    //- Communications type of this stream
    commsTypes commsType_;
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("UPstream");
  // Static data
    //- Should compact transfer be used in which floats replace doubles
    //  reducing the bandwidth requirement at the expense of some loss
    //  in accuracy
    static bool floatTransfer;
    //- Number of processors at which the sum algorithm changes from linear
    //  to tree
    static int nProcsSimpleSum;
    //- Default commsType
    static commsTypes defaultCommsType;
    //- Number of polling cycles in processor updates
    static int nPollProcInterfaces;
    //- Default communicator (all processors)
    static label worldComm;
    //- Debugging: warn for use of any communicator differing from warnComm
    static label warnComm;
  // Constructors
    //- Construct given optional buffer size
    UPstream(const commsTypes commsType)
    :
      commsType_(commsType)
    {}
  // Member functions
    //- Allocate a new communicator
    static label allocateCommunicator
    (
      const label parent,
      const labelList& subRanks,
      const bool doPstream = true
    );
    //- Free a previously allocated communicator
    static void freeCommunicator
    (
      const label communicator,
      const bool doPstream = true
    );
    //- Free all communicators
    static void freeCommunicators(const bool doPstream);
    //- Helper class for allocating/freeing communicators
    class communicator
    {
      label comm_;
      //- Disallow copy and assignment
      communicator(const communicator&);
      void operator=(const communicator&);
    public:
      communicator
      (
        const label parent,
        const labelList& subRanks,
        const bool doPstream
      )
      :
        comm_(allocateCommunicator(parent, subRanks, doPstream))
      {}
      ~communicator()
      {
        freeCommunicator(comm_);
      }
      operator label() const
      {
        return comm_;
      }
    };
    //- Return physical processor number (i.e. processor number in
    //  worldComm) given communicator and procssor
    static int baseProcNo(const label myComm, const int procID);
    //- Return processor number in communicator (given physical processor
    //  number) (= reverse of baseProcNo)
    static label procNo(const label comm, const int baseProcID);
    //- Return processor number in communicator (given processor number
    //  and communicator)
    static label procNo
    (
      const label myComm,
      const label currentComm,
      const int currentProcID
    );
    //- Add the valid option this type of communications library
    //  adds/requires on the command line
    static void addValidParOptions(HashTable<string>& validParOptions);
    //- Initialisation function called from main
    //  Spawns slave processes and initialises inter-communication
    static bool init(int& argc, char**& argv);
    // Non-blocking comms
      //- Get number of outstanding requests
      static label nRequests();
      //- Truncate number of outstanding requests
      static void resetRequests(const label sz);
      //- Wait until all requests (from start onwards) have finished.
      static void waitRequests(const label start = 0);
      //- Wait until request i has finished.
      static void waitRequest(const label i);
      //- Non-blocking comms: has request i finished?
      static bool finishedRequest(const label i);
      static int allocateTag(const char*);
      static int allocateTag(const word&);
      static void freeTag(const char*, const int tag);
      static void freeTag(const word&, const int tag);
    //- Is this a parallel run?
    static bool& parRun()
    {
      return parRun_;
    }
    //- Number of processes in parallel run
    static label nProcs(const label communicator = 0)
    {
      return procIDs_[communicator].size();
    }
    //- Process index of the master
    static int masterNo()
    {
      return 0;
    }
    //- Am I the master process
    static bool master(const label communicator = 0)
    {
      return myProcNo_[communicator] == masterNo();
    }
    //- Number of this process (starting from masterNo() = 0)
    static int myProcNo(const label communicator = 0)
    {
      return myProcNo_[communicator];
    }
    static label parent(const label communicator)
    {
      return parentCommunicator_(communicator);
    }
    //- Process ID of given process index
    static List<int>& procID(label communicator)
    {
      return procIDs_[communicator];
    }
    //- Process index of first slave
    static int firstSlave()
    {
      return 1;
    }
    //- Process index of last slave
    static int lastSlave(const label communicator = 0)
    {
      return nProcs(communicator) - 1;
    }
    //- Communication schedule for linear all-to-master (proc 0)
    static const List<commsStruct>& linearCommunication
    (
      const label communicator = 0
    )
    {
      return linearCommunication_[communicator];
    }
    //- Communication schedule for tree all-to-master (proc 0)
    static const List<commsStruct>& treeCommunication
    (
      const label communicator = 0
    )
    {
      return treeCommunication_[communicator];
    }
    //- Message tag of standard messages
    static int& msgType()
    {
      return msgType_;
    }
      //- Get the communications type of the stream
      commsTypes commsType() const
      {
        return commsType_;
      }
      //- Set the communications type of the stream
      commsTypes commsType(const commsTypes ct)
      {
        commsTypes oldCommsType = commsType_;
        commsType_ = ct;
        return oldCommsType;
      }
    //- Exit program
    static void exit(int errnum = 1);
    //- Abort program
    static void abort();
};
Ostream& operator<<(Ostream&, const UPstream::commsStruct&);
}  // namespace mousse
#endif
