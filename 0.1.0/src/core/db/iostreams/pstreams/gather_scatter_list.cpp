// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ipstream.hpp"
#include "opstream.hpp"
#include "contiguous.hpp"

namespace mousse
{

// Member Functions 
template<class T>
void Pstream::gatherList
(
  const List<UPstream::commsStruct>& comms,
  List<T>& Values,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) > 1)
  {
    if (Values.size() != UPstream::nProcs(comm))
    {
      FATAL_ERROR_IN
      (
        "UPstream::gatherList(const List<UPstream::commsStruct>&"
        ", List<T>)"
      )
      << "Size of list:" << Values.size()
      << " does not equal the number of processors:"
      << UPstream::nProcs(comm)
      << mousse::abort(FatalError);
    }
    // Get my communication order
    const commsStruct& myComm = comms[UPstream::myProcNo(comm)];
    // Receive from my downstairs neighbours
    FOR_ALL(myComm.below(), belowI)
    {
      label belowID = myComm.below()[belowI];
      const labelList& belowLeaves = comms[belowID].allBelow();
      if (contiguous<T>())
      {
        List<T> receivedValues(belowLeaves.size() + 1);
        UIPstream::read
        (
          UPstream::scheduled,
          belowID,
          reinterpret_cast<char*>(receivedValues.begin()),
          receivedValues.byteSize(),
          tag,
          comm
        );
        Values[belowID] = receivedValues[0];
        FOR_ALL(belowLeaves, leafI)
        {
          Values[belowLeaves[leafI]] = receivedValues[leafI + 1];
        }
      }
      else
      {
        IPstream fromBelow(UPstream::scheduled, belowID, 0, tag, comm);
        fromBelow >> Values[belowID];
        if (debug & 2)
        {
          Pout<< " received through "
            << belowID << " data from:" << belowID
            << " data:" << Values[belowID] << endl;
        }
        // Receive from all other processors below belowID
        FOR_ALL(belowLeaves, leafI)
        {
          label leafID = belowLeaves[leafI];
          fromBelow >> Values[leafID];
          if (debug & 2)
          {
            Pout<< " received through "
              << belowID << " data from:" << leafID
              << " data:" << Values[leafID] << endl;
          }
        }
      }
    }
    // Send up from Values:
    // - my own value first
    // - all belowLeaves next
    if (myComm.above() != -1)
    {
      const labelList& belowLeaves = myComm.allBelow();
      if (debug & 2)
      {
        Pout<< " sending to " << myComm.above()
          << " data from me:" << UPstream::myProcNo(comm)
          << " data:" << Values[UPstream::myProcNo(comm)] << endl;
      }
      if (contiguous<T>())
      {
        List<T> sendingValues(belowLeaves.size() + 1);
        sendingValues[0] = Values[UPstream::myProcNo(comm)];
        FOR_ALL(belowLeaves, leafI)
        {
          sendingValues[leafI + 1] = Values[belowLeaves[leafI]];
        }
        OPstream::write
        (
          UPstream::scheduled,
          myComm.above(),
          reinterpret_cast<const char*>(sendingValues.begin()),
          sendingValues.byteSize(),
          tag,
          comm
        );
      }
      else
      {
        OPstream toAbove
        (
          UPstream::scheduled,
          myComm.above(),
          0,
          tag,
          comm
        );
        toAbove << Values[UPstream::myProcNo(comm)];
        FOR_ALL(belowLeaves, leafI)
        {
          label leafID = belowLeaves[leafI];
          if (debug & 2)
          {
            Pout<< " sending to "
              << myComm.above() << " data from:" << leafID
              << " data:" << Values[leafID] << endl;
          }
          toAbove << Values[leafID];
        }
      }
    }
  }
}


template <class T>
void Pstream::gatherList(List<T>& Values, const int tag, const label comm)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum)
  {
    gatherList(UPstream::linearCommunication(comm), Values, tag, comm);
  }
  else
  {
    gatherList(UPstream::treeCommunication(comm), Values, tag, comm);
  }
}


template<class T>
void Pstream::scatterList
(
  const List<UPstream::commsStruct>& comms,
  List<T>& Values,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) > 1)
  {
    if (Values.size() != UPstream::nProcs(comm))
    {
      FATAL_ERROR_IN
      (
        "UPstream::scatterList(const List<UPstream::commsStruct>&"
        ", List<T>)"
      )
      << "Size of list:" << Values.size()
      << " does not equal the number of processors:"
      << UPstream::nProcs(comm)
      << mousse::abort(FatalError);
    }
    // Get my communication order
    const commsStruct& myComm = comms[UPstream::myProcNo(comm)];
    // Reveive from up
    if (myComm.above() != -1)
    {
      const labelList& notBelowLeaves = myComm.allNotBelow();
      if (contiguous<T>())
      {
        List<T> receivedValues(notBelowLeaves.size());
        UIPstream::read
        (
          UPstream::scheduled,
          myComm.above(),
          reinterpret_cast<char*>(receivedValues.begin()),
          receivedValues.byteSize(),
          tag,
          comm
        );
        FOR_ALL(notBelowLeaves, leafI)
        {
          Values[notBelowLeaves[leafI]] = receivedValues[leafI];
        }
      }
      else
      {
        IPstream fromAbove
        (
          UPstream::scheduled,
          myComm.above(),
          0,
          tag,
          comm
        );
        FOR_ALL(notBelowLeaves, leafI)
        {
          label leafID = notBelowLeaves[leafI];
          fromAbove >> Values[leafID];
          if (debug)
          {
            Pout<< " received through "
              << myComm.above() << " data for:" << leafID
              << " data:" << Values[leafID] << endl;
          }
        }
      }
    }
    // Send to my downstairs neighbours
    FOR_ALL(myComm.below(), belowI)
    {
      label belowID = myComm.below()[belowI];
      const labelList& notBelowLeaves = comms[belowID].allNotBelow();
      if (contiguous<T>())
      {
        List<T> sendingValues(notBelowLeaves.size());
        FOR_ALL(notBelowLeaves, leafI)
        {
          sendingValues[leafI] = Values[notBelowLeaves[leafI]];
        }
        OPstream::write
        (
          UPstream::scheduled,
          belowID,
          reinterpret_cast<const char*>(sendingValues.begin()),
          sendingValues.byteSize(),
          tag,
          comm
        );
      }
      else
      {
        OPstream toBelow(UPstream::scheduled, belowID, 0, tag, comm);
        // Send data destined for all other processors below belowID
        FOR_ALL(notBelowLeaves, leafI)
        {
          label leafID = notBelowLeaves[leafI];
          toBelow << Values[leafID];
          if (debug)
          {
            Pout<< " sent through "
              << belowID << " data for:" << leafID
              << " data:" << Values[leafID] << endl;
          }
        }
      }
    }
  }
}


template <class T>
void Pstream::scatterList(List<T>& Values, const int tag, const label comm)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum)
  {
    scatterList(UPstream::linearCommunication(comm), Values, tag, comm);
  }
  else
  {
    scatterList(UPstream::treeCommunication(comm), Values, tag, comm);
  }
}

}  // namespace mousse
