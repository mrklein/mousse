#ifndef CORE_MESHES_TPROCESSOR_TOPOLOGY_HPP_
#define CORE_MESHES_TPROCESSOR_TOPOLOGY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ProcessorTopology
// Description
//   Determines processor-processor connection. After instantiation contains
//   on all processors the processor-processor connection table.
//   *this[procI] gives the list of neighbouring processors.
//   TODO: This does not currently correctly support multiple processor
//   patches connecting two processors.
// SourceFiles
//   processor_topology.cpp
#include "label_list.hpp"
#include "ldu_schedule.hpp"
namespace mousse
{
template<class Container, class ProcPatch>
class ProcessorTopology
:
  public labelListList
{
private:
  // Private data
    //- Local map from neighbour proc to patchI. Different per processor!
    //  -1 or patchI for connection to procID
    labelList procPatchMap_;
    //- Order in which the patches should be initialised/evaluated
    //  corresponding to the schedule
    lduSchedule patchSchedule_;
  // Private Member Functions
    //- Return all neighbouring processors of this processor. Set
    //  procPatchMap_.
    labelList procNeighbours(const label nProcs, const Container&);
public:
  // Constructors
    //- Construct from boundaryMesh
    ProcessorTopology(const Container& patches, const label comm);
  // Member Functions
    //- From neighbour processor to index in boundaryMesh. Local information
    //  (so not same over all processors)
    const labelList& procPatchMap() const
    {
      return procPatchMap_;
    }
    //- Order in which the patches should be initialised/evaluated
    //  corresponding to the schedule
    const lduSchedule& patchSchedule() const
    {
      return patchSchedule_;
    }
    //- Calculate non-blocking (i.e. unscheduled) schedule
    static lduSchedule nonBlockingSchedule(const Container& patches);
};
}  // namespace mousse
#ifdef NoRepository
#   include "_processor_topology.cpp"
#endif
#endif
