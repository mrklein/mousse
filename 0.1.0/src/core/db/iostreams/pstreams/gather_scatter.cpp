// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uopstream.hpp"
#include "opstream.hpp"
#include "uipstream.hpp"
#include "ipstream.hpp"
#include "contiguous.hpp"

namespace mousse
{

// Member Functions 
template<class T, class BinaryOp>
void Pstream::gather
(
  const List<UPstream::commsStruct>& comms,
  T& Value,
  const BinaryOp& bop,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) > 1)
  {
    // Get my communication order
    const commsStruct& myComm = comms[UPstream::myProcNo(comm)];
    // Receive from my downstairs neighbours
    FOR_ALL(myComm.below(), belowI)
    {
      T value;
      if (contiguous<T>())
      {
        UIPstream::read
        (
          UPstream::scheduled,
          myComm.below()[belowI],
          reinterpret_cast<char*>(&value),
          sizeof(T),
          tag,
          comm
        );
      }
      else
      {
        IPstream fromBelow
        (
          UPstream::scheduled,
          myComm.below()[belowI],
          0,
          tag,
          comm
        );
        fromBelow >> value;
      }
      Value = bop(Value, value);
    }
    // Send up Value
    if (myComm.above() != -1)
    {
      if (contiguous<T>())
      {
        UOPstream::write
        (
          UPstream::scheduled,
          myComm.above(),
          reinterpret_cast<const char*>(&Value),
          sizeof(T),
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
        toAbove << Value;
      }
    }
  }
}


template <class T, class BinaryOp>
void Pstream::gather
(
  T& Value,
  const BinaryOp& bop,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum)
  {
    gather(UPstream::linearCommunication(comm), Value, bop, tag, comm);
  }
  else
  {
    gather(UPstream::treeCommunication(comm), Value, bop, tag, comm);
  }
}


template<class T>
void Pstream::scatter
(
  const List<UPstream::commsStruct>& comms,
  T& Value,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) > 1)
  {
    // Get my communication order
    const commsStruct& myComm = comms[UPstream::myProcNo(comm)];
    // Reveive from up
    if (myComm.above() != -1)
    {
      if (contiguous<T>())
      {
        UIPstream::read
        (
          UPstream::scheduled,
          myComm.above(),
          reinterpret_cast<char*>(&Value),
          sizeof(T),
          tag,
          comm
        );
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
        fromAbove >> Value;
      }
    }
    // Send to my downstairs neighbours
    FOR_ALL(myComm.below(), belowI)
    {
      if (contiguous<T>())
      {
        UOPstream::write
        (
          UPstream::scheduled,
          myComm.below()[belowI],
          reinterpret_cast<const char*>(&Value),
          sizeof(T),
          tag,
          comm
        );
      }
      else
      {
        OPstream toBelow
        (
          UPstream::scheduled,
          myComm.below()[belowI],
          0,
          tag,
          comm
        );
        toBelow << Value;
      }
    }
  }
}


template <class T>
void Pstream::scatter(T& Value, const int tag, const label comm)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum)
  {
    scatter(UPstream::linearCommunication(comm), Value, tag, comm);
  }
  else
  {
    scatter(UPstream::treeCommunication(comm), Value, tag, comm);
  }
}

}  // namespace mousse
