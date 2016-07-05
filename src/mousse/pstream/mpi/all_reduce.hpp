#ifndef PSTREAM_MPI_ALL_REDUCE_HPP_
#define PSTREAM_MPI_ALL_REDUCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "upstream.hpp"
#include <mpi.h>


namespace mousse {

template<class Type, class BinaryOp>
void allReduce
(
  Type& Value,
  int count,
  MPI_Datatype MPIType,
  MPI_Op op,
  const BinaryOp& bop,
  const int tag,
  const label communicator
);

}  // namespace mousse

#include "all_reduce.ipp"

#endif
