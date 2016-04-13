// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pstream_globals.hpp"
#include "iostreams.hpp"


namespace mousse {

// Static Data Members

// Outstanding non-blocking operations.
//! \cond fileScope
DynamicList<MPI_Request> PstreamGlobals::outstandingRequests_;
//! \endcond

// Max outstanding message tag operations.
//! \cond fileScope
int PstreamGlobals::nTags_ = 0;
//! \endcond

// Free'd message tags
//! \cond fileScope
DynamicList<int> PstreamGlobals::freedTags_;
//! \endcond

// Allocated communicators.
//! \cond fileScope
DynamicList<MPI_Comm> PstreamGlobals::MPICommunicators_;
DynamicList<MPI_Group> PstreamGlobals::MPIGroups_;
//! \endcond

void PstreamGlobals::checkCommunicator
(
  const label comm,
  const label otherProcNo
)
{
  if (comm < 0 || comm >= PstreamGlobals::MPICommunicators_.size()) {
    FATAL_ERROR_IN
    (
      "PstreamGlobals::checkCommunicator(const label, const label)"
    )
    << "otherProcNo:" << otherProcNo << " : illegal communicator "
    << comm << endl
    << "Communicator should be within range 0.."
    << PstreamGlobals::MPICommunicators_.size()-1 << abort(FatalError);
  }
}

}  // namespace mousse

