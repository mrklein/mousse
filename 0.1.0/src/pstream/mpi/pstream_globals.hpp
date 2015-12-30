// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   pstream_globals.cpp
#ifndef pstream_globals_hpp_
#define pstream_globals_hpp_
#include "dynamic_list.hpp"
#include <mpi.h>
namespace mousse
{
namespace PstreamGlobals
{
extern DynamicList<MPI_Request> outstandingRequests_;
//extern int nRequests_;
//extern DynamicList<label> freedRequests_;
extern int nTags_;
extern DynamicList<int> freedTags_;
// Current communicators. First element will be MPI_COMM_WORLD
extern DynamicList<MPI_Comm> MPICommunicators_;
extern DynamicList<MPI_Group> MPIGroups_;
void checkCommunicator(const label, const label procNo);
};
}  // namespace mousse
#endif
