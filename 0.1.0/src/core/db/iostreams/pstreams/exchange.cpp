// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pstream.hpp"
#include "contiguous.hpp"
#include "pstream_combine_reduce_ops.hpp"
#include "upstream.hpp"

// Member Functions 
template<class Container, class T>
void mousse::Pstream::exchange
(
  const List<Container>& sendBufs,
  List<Container>& recvBufs,
  labelListList& sizes,
  const int tag,
  const label comm,
  const bool block
)
{
  if (!contiguous<T>())
  {
    FATAL_ERROR_IN
    (
      "Pstream::exchange(..)"
    )
    << "Continuous data only." << sizeof(T) << mousse::abort(FatalError);
  }
  if (sendBufs.size() != UPstream::nProcs(comm))
  {
    FATAL_ERROR_IN
    (
      "Pstream::exchange(..)"
    )
    << "Size of list:" << sendBufs.size()
    << " does not equal the number of processors:"
    << UPstream::nProcs(comm)
    << mousse::abort(FatalError);
  }
  sizes.setSize(UPstream::nProcs(comm));
  labelList& nsTransPs = sizes[UPstream::myProcNo(comm)];
  nsTransPs.setSize(UPstream::nProcs(comm));
  FOR_ALL(sendBufs, procI)
  {
    nsTransPs[procI] = sendBufs[procI].size();
  }
  // Send sizes across. Note: blocks.
  combineReduce(sizes, UPstream::listEq(), tag, comm);
  if (UPstream::nProcs(comm) > 1)
  {
    label startOfRequests = Pstream::nRequests();
    // Set up receives
    // ~~~~~~~~~~~~~~~
    recvBufs.setSize(sendBufs.size());
    FOR_ALL(sizes, procI)
    {
      label nRecv = sizes[procI][UPstream::myProcNo(comm)];
      if (procI != Pstream::myProcNo(comm) && nRecv > 0)
      {
        recvBufs[procI].setSize(nRecv);
        UIPstream::read
        (
          UPstream::nonBlocking,
          procI,
          reinterpret_cast<char*>(recvBufs[procI].begin()),
          nRecv*sizeof(T),
          tag,
          comm
        );
      }
    }
    // Set up sends
    // ~~~~~~~~~~~~
    FOR_ALL(sendBufs, procI)
    {
      if (procI != Pstream::myProcNo(comm) && sendBufs[procI].size() > 0)
      {
        if
        (
         !UOPstream::write
          (
            UPstream::nonBlocking,
            procI,
            reinterpret_cast<const char*>(sendBufs[procI].begin()),
            sendBufs[procI].size()*sizeof(T),
            tag,
            comm
          )
        )
        {
          FATAL_ERROR_IN("Pstream::exchange(..)")
            << "Cannot send outgoing message. "
            << "to:" << procI << " nBytes:"
            << label(sendBufs[procI].size()*sizeof(T))
            << mousse::abort(FatalError);
        }
      }
    }
    // Wait for all to finish
    // ~~~~~~~~~~~~~~~~~~~~~~
    if (block)
    {
      Pstream::waitRequests(startOfRequests);
    }
  }
  // Do myself
  recvBufs[Pstream::myProcNo(comm)] = sendBufs[Pstream::myProcNo(comm)];
}
