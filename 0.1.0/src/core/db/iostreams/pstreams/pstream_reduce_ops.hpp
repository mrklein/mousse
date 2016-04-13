#ifndef CORE_DB_IOSTREAMS_PSTREAMS_PSTREAM_REDUCE_OPS_HPP_
#define CORE_DB_IOSTREAMS_PSTREAMS_PSTREAM_REDUCE_OPS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pstream.hpp"
#include "ops.hpp"
#include "vector_2d.hpp"
#include "iostreams.hpp"


namespace mousse {

// Reduce operation with user specified communication schedule
template<class T, class BinaryOp>
void reduce
(
  const List<UPstream::commsStruct>& comms,
  T& Value,
  const BinaryOp& bop,
  const int tag,
  const label comm
)
{
  if (UPstream::warnComm != -1 && comm != UPstream::warnComm) {
    Pout << "** reducing:" << Value << " with comm:" << comm
      << endl;
    error::printStack(Pout);
  }
  Pstream::gather(comms, Value, bop, tag, comm);
  Pstream::scatter(comms, Value, tag, comm);
}


// Reduce using either linear or tree communication schedule
template<class T, class BinaryOp>
void reduce
(
  T& Value,
  const BinaryOp& bop,
  const int tag = Pstream::msgType(),
  const label comm = UPstream::worldComm
)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum) {
    reduce(UPstream::linearCommunication(comm), Value, bop, tag, comm);
  } else {
    reduce(UPstream::treeCommunication(comm), Value, bop, tag, comm);
  }
}


// Reduce using either linear or tree communication schedule
template<class T, class BinaryOp>
T returnReduce
(
  const T& Value,
  const BinaryOp& bop,
  const int tag = Pstream::msgType(),
  const label comm = UPstream::worldComm
)
{
  T WorkValue(Value);
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum) {
    reduce
    (
      UPstream::linearCommunication(comm),
      WorkValue,
      bop,
      tag,
      comm
    );
  } else {
    reduce
    (
      UPstream::treeCommunication(comm),
      WorkValue,
      bop,
      tag,
      comm
    );
  }
  return WorkValue;
}


// Reduce with sum of both value and count (for averaging)
template<class T>
void sumReduce
(
  T& Value,
  label& Count,
  const int tag = Pstream::msgType(),
  const label comm = UPstream::worldComm
)
{
  reduce(Value, sumOp<T>(), tag, comm);
  reduce(Count, sumOp<label>(), tag, comm);
}


// Non-blocking version of reduce. Sets request.
template<class T, class BinaryOp>
void reduce
(
  T&,
  const BinaryOp&,
  const int /*tag*/,
  const label /*comm*/,
  label& /*request*/
)
{
  NOT_IMPLEMENTED
  (
    "reduce(T&, const BinaryOp&, const int, const label, label&"
  );
}


// Insist there are specialisations for the common reductions of scalar(s)
void reduce
(
  scalar& Value,
  const sumOp<scalar>& bop,
  const int tag = Pstream::msgType(),
  const label comm = UPstream::worldComm
);


void reduce
(
  scalar& Value,
  const minOp<scalar>& bop,
  const int tag = Pstream::msgType(),
  const label comm = UPstream::worldComm
);


void reduce
(
  vector2D& Value,
  const sumOp<vector2D>& bop,
  const int tag = Pstream::msgType(),
  const label comm = UPstream::worldComm
);


void sumReduce
(
  scalar& Value,
  label& Count,
  const int tag = Pstream::msgType(),
  const label comm = UPstream::worldComm
);


void reduce
(
  scalar& Value,
  const sumOp<scalar>& bop,
  const int tag,
  const label comm,
  label& request
);

}  // namespace mousse

#endif
