// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef pstream_combine_reduce_ops_hpp_
#define pstream_combine_reduce_ops_hpp_
#include "upstream.hpp"
#include "pstream.hpp"
#include "ops.hpp"
namespace mousse
{
template<class T, class CombineOp>
void combineReduce
(
  const List<UPstream::commsStruct>& comms,
  T& Value,
  const CombineOp& cop,
  const int tag,
  const label comm
)
{
  Pstream::combineGather(comms, Value, cop, tag, comm);
  Pstream::combineScatter(comms, Value, tag, comm);
}
template<class T, class CombineOp>
void combineReduce
(
  T& Value,
  const CombineOp& cop,
  const int tag = Pstream::msgType(),
  const label comm = Pstream::worldComm
)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum)
  {
    Pstream::combineGather
    (
      UPstream::linearCommunication(comm),
      Value,
      cop,
      tag,
      comm
    );
    Pstream::combineScatter
    (
      UPstream::linearCommunication(comm),
      Value,
      tag,
      comm
    );
  }
  else
  {
    Pstream::combineGather
    (
      UPstream::treeCommunication(comm),
      Value,
      cop,
      tag,
      comm
    );
    Pstream::combineScatter
    (
      UPstream::treeCommunication(comm),
      Value,
      tag,
      comm
    );
  }
}
}  // namespace mousse
#endif
