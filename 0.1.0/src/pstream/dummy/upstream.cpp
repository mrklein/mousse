// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "upstream.hpp"
#include "pstream_reduce_ops.hpp"
void mousse::UPstream::addValidParOptions(HashTable<string>& validParOptions)
{}
bool mousse::UPstream::init(int& argc, char**& argv)
{
  FatalErrorIn("UPstream::init(int& argc, char**& argv)")
    << "Trying to use the dummy Pstream library." << nl
    << "This dummy library cannot be used in parallel mode"
    << mousse::exit(FatalError);
  return false;
}
void mousse::UPstream::exit(int errnum)
{
  notImplemented("UPstream::exit(int errnum)");
}
void mousse::UPstream::abort()
{
  notImplemented("UPstream::abort()");
}
void mousse::reduce(scalar&, const sumOp<scalar>&, const int, const label)
{}
void mousse::reduce(scalar&, const minOp<scalar>&, const int, const label)
{}
void mousse::reduce(vector2D&, const sumOp<vector2D>&, const int, const label)
{}
void mousse::sumReduce
(
  scalar&,
  label&,
  const int,
  const label
)
{}
void mousse::reduce(scalar&, const sumOp<scalar>&, const int, const label, label&)
{}
void mousse::UPstream::allocatePstreamCommunicator
(
  const label,
  const label
)
{}
void mousse::UPstream::freePstreamCommunicator(const label)
{}
mousse::label mousse::UPstream::nRequests()
{
  return 0;
}
void mousse::UPstream::resetRequests(const label i)
{}
void mousse::UPstream::waitRequests(const label start)
{}
void mousse::UPstream::waitRequest(const label i)
{}
bool mousse::UPstream::finishedRequest(const label i)
{
  notImplemented("UPstream::finishedRequest()");
  return false;
}
