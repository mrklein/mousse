// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_processor_topology.hpp"
#include "list_ops.hpp"
#include "pstream.hpp"
#include "comm_schedule.hpp"
#include "bool_list.hpp"


// Private Member Functions 
template<class Container, class ProcPatch>
mousse::labelList mousse::ProcessorTopology<Container, ProcPatch>::procNeighbours
(
  const label nProcs,
  const Container& patches
)
{
  // Determine number of processor neighbours and max neighbour id.
  label nNeighbours = 0;
  label maxNb = 0;
  boolList isNeighbourProc{nProcs, false};
  FOR_ALL(patches, patchi) {
    const typename Container::const_reference patch = patches[patchi];
    if (!isA<ProcPatch>(patch))
      continue;
    const ProcPatch& procPatch = refCast<const ProcPatch>(patch);
    label pNeighbProcNo = procPatch.neighbProcNo();
    if (!isNeighbourProc[pNeighbProcNo]) {
      nNeighbours++;
      maxNb = max(maxNb, procPatch.neighbProcNo());
      isNeighbourProc[pNeighbProcNo] = true;
    }
  }
  labelList neighbours{nNeighbours, -1};
  nNeighbours = 0;
  FOR_ALL(isNeighbourProc, procI) {
    if (isNeighbourProc[procI]) {
      neighbours[nNeighbours++] = procI;
    }
  }
  procPatchMap_.setSize(maxNb + 1);
  procPatchMap_ = -1;
  FOR_ALL(patches, patchi) {
    const typename Container::const_reference patch = patches[patchi];
    if (isA<ProcPatch>(patch)) {
      const ProcPatch& procPatch = refCast<const ProcPatch>(patch);
      // Construct reverse map
      procPatchMap_[procPatch.neighbProcNo()] = patchi;
    }
  }
  return neighbours;
}


// Constructors 
// Construct from components
template<class Container, class ProcPatch>
mousse::ProcessorTopology<Container, ProcPatch>::ProcessorTopology
(
  const Container& patches,
  const label comm
)
:
  labelListList{Pstream::nProcs(comm)},
  patchSchedule_{2*patches.size()}
{
  if (Pstream::parRun()) {
    // Fill my 'slot' with my neighbours
    operator[](Pstream::myProcNo(comm)) = procNeighbours(this->size(), patches);
    // Distribute to all processors
    Pstream::gatherList(*this, Pstream::msgType(), comm);
    Pstream::scatterList(*this, Pstream::msgType(), comm);
  }
  if (Pstream::parRun() && Pstream::defaultCommsType == Pstream::scheduled) {
    label patchEvali = 0;
    // 1. All non-processor patches
    FOR_ALL(patches, patchi) {
      if (!isA<ProcPatch>(patches[patchi])) {
        patchSchedule_[patchEvali].patch = patchi;
        patchSchedule_[patchEvali++].init = true;
        patchSchedule_[patchEvali].patch = patchi;
        patchSchedule_[patchEvali++].init = false;
      }
    }
    // 2. All processor patches
    // ~~~~~~~~~~~~~~~~~~~~~~~~
    // Determine the schedule for all. Insert processor pair once
    // to determine the schedule. Each processor pair stands for both
    // send and receive.
    label nComms = 0;
    FOR_ALL(*this, procI) {
      nComms += operator[](procI).size();
    }
    DynamicList<labelPair> comms{nComms};
    FOR_ALL(*this, procI) {
      const labelList& nbrs = operator[](procI);
      FOR_ALL(nbrs, i) {
        if (procI < nbrs[i]) {
          comms.append(labelPair(procI, nbrs[i]));
        }
      }
    }
    comms.shrink();
    // Determine a schedule.
    labelList mySchedule
    {
      commSchedule
      (
        Pstream::nProcs(comm),
        comms
      ).procSchedule()[Pstream::myProcNo(comm)]
    };
    FOR_ALL(mySchedule, iter) {
      label commI = mySchedule[iter];
      // Get the other processor
      label nb = comms[commI][0];
      if (nb == Pstream::myProcNo(comm)) {
        nb = comms[commI][1];
      }
      label patchi = procPatchMap_[nb];
      if (Pstream::myProcNo(comm) > nb) {
        patchSchedule_[patchEvali].patch = patchi;
        patchSchedule_[patchEvali++].init = true;
        patchSchedule_[patchEvali].patch = patchi;
        patchSchedule_[patchEvali++].init = false;
      } else {
        patchSchedule_[patchEvali].patch = patchi;
        patchSchedule_[patchEvali++].init = false;
        patchSchedule_[patchEvali].patch = patchi;
        patchSchedule_[patchEvali++].init = true;
      }
    }
  } else {
    patchSchedule_ = nonBlockingSchedule(patches);
  }
}


// Member Functions 
template<class Container, class ProcPatch>
mousse::lduSchedule
mousse::ProcessorTopology<Container, ProcPatch>::nonBlockingSchedule
(
  const Container& patches
)
{
  lduSchedule patchSchedule{2*patches.size()};
  label patchEvali = 0;
  // 1. All non-processor patches
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Have evaluate directly after initEvaluate. Could have them separated
  // as long as they're not intermingled with processor patches since
  // then e.g. any reduce parallel traffic would interfere with the
  // processor swaps.
  FOR_ALL(patches, patchi) {
    if (!isA<ProcPatch>(patches[patchi])) {
      patchSchedule[patchEvali].patch = patchi;
      patchSchedule[patchEvali++].init = true;
      patchSchedule[patchEvali].patch = patchi;
      patchSchedule[patchEvali++].init = false;
    }
  }
  // 2. All processor patches
  // ~~~~~~~~~~~~~~~~~~~~~~~~
  // 2a. initEvaluate
  FOR_ALL(patches, patchi) {
    if (isA<ProcPatch>(patches[patchi])) {
      patchSchedule[patchEvali].patch = patchi;
      patchSchedule[patchEvali++].init = true;
    }
  }
  // 2b. evaluate
  FOR_ALL(patches, patchi) {
    if (isA<ProcPatch>(patches[patchi])) {
      patchSchedule[patchEvali].patch = patchi;
      patchSchedule[patchEvali++].init = false;
    }
  }
  return patchSchedule;
}
