// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "all_reduce.hpp"


// Global Functions 
template<class Type, class BinaryOp>
void mousse::allReduce
(
  Type& Value,
  int MPICount,
  MPI_Datatype MPIType,
  MPI_Op MPIOp,
  const BinaryOp& bop,
  const int tag,
  const label communicator
)
{
  if (!UPstream::parRun()) {
    return;
  }
  if (UPstream::nProcs(communicator) <= UPstream::nProcsSimpleSum) {
    if (UPstream::master(communicator)) {
      for
      (
        int slave=UPstream::firstSlave();
        slave<=UPstream::lastSlave(communicator);
        slave++
      ) {
        Type value;
        if (
          MPI_Recv
          (
            &value,
            MPICount,
            MPIType,
            slave,  //UPstream::procID(slave),
            tag,
            PstreamGlobals::MPICommunicators_[communicator],
            MPI_STATUS_IGNORE
          )
        )
        {
          FATAL_ERROR_IN
          (
            "void mousse::allReduce\n"
            "(\n"
            "    Type&,\n"
            "    int,\n"
            "    MPI_Datatype,\n"
            "    MPI_Op,\n"
            "    const BinaryOp&,\n"
            "    const int\n"
            ")\n"
          )
          << "MPI_Recv failed"
          << mousse::abort(FatalError);
        }
        Value = bop(Value, value);
      }
    } else {
      if
      (
        MPI_Send
        (
          &Value,
          MPICount,
          MPIType,
          UPstream::masterNo(),//UPstream::procID(masterNo()),
          tag,
          PstreamGlobals::MPICommunicators_[communicator]
        )
      ) {
        FATAL_ERROR_IN
        (
          "void mousse::allReduce\n"
          "(\n"
          "    Type&,\n"
          "    int,\n"
          "    MPI_Datatype,\n"
          "    MPI_Op,\n"
          "    const BinaryOp&,\n"
          "    const int\n"
          ")\n"
        )
        << "MPI_Send failed"
        << mousse::abort(FatalError);
      }
    }
    if (UPstream::master(communicator)) {
      for
      (
        int slave=UPstream::firstSlave();
        slave<=UPstream::lastSlave(communicator);
        slave++
      ) {
        if
        (
          MPI_Send
          (
            &Value,
            MPICount,
            MPIType,
            slave,      //UPstream::procID(slave),
            tag,
            PstreamGlobals::MPICommunicators_[communicator]
          )
        ) {
          FATAL_ERROR_IN
          (
            "void mousse::allReduce\n"
            "(\n"
            "    Type&,\n"
            "    int,\n"
            "    MPI_Datatype,\n"
            "    MPI_Op,\n"
            "    const BinaryOp&,\n"
            "    const int\n"
            ")\n"
          )
          << "MPI_Send failed"
          << mousse::abort(FatalError);
        }
      }
    } else {
      if
      (
        MPI_Recv
        (
          &Value,
          MPICount,
          MPIType,
          UPstream::masterNo(),
          tag,
          PstreamGlobals::MPICommunicators_[communicator],
          MPI_STATUS_IGNORE
        )
      ) {
        FATAL_ERROR_IN
        (
          "void mousse::allReduce\n"
          "(\n"
          "    Type&,\n"
          "    int,\n"
          "    MPI_Datatype,\n"
          "    MPI_Op,\n"
          "    const BinaryOp&,\n"
          "    const int\n"
          ")\n"
        )
        << "MPI_Recv failed"
        << mousse::abort(FatalError);
      }
    }
  } else {
    Type sum;
    MPI_Allreduce
    (
      &Value,
      &sum,
      MPICount,
      MPIType,
      MPIOp,
      PstreamGlobals::MPICommunicators_[communicator]
    );
    Value = sum;
  }
}
