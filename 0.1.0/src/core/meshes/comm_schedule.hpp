#ifndef CORE_MESHES_COMM_SCHEDULE_HPP_
#define CORE_MESHES_COMM_SCHEDULE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::commSchedule
// Description
//   Determines the order in which a set of processors should communicate
//   with one another.
//   The communication order should
//    - have maximum overlap
//    - allow blocking communication without deadlock
//   Does a very simple scheduling which assumes same time for all operations.
//   After construction:
//    - schedule() gives the order in which the input communication should occur
//    - procSchedule()[procI] gives per procI
//   Does not care whether 'talking' is first send, second receive or maybe
//   full swap. This is all responsability of caller. See ProcessorTopology
//   class for use in scheduling processor boundary swaps.

#include "dynamic_list.hpp"
#include "label_pair.hpp"
#include "label_list.hpp"


namespace mousse {

class commSchedule
{
  // Private data
    //- Order in which input communication has been scheduled
    labelList schedule_;
    //- Per processor the order in which communication has been scheduled
    labelListList procSchedule_;
  // Private Member Functions
    //- Count the number of outstanding communications for a single
    //  processor
    label outstandingComms(const labelList&, DynamicList<label>&) const;
public:
  CLASS_NAME("commSchedule");
  // Constructors
    //- Construct from wanted communication. Wanted communication is between
    //  two processors. Can be a one-way communication or
    //  two-way communication, that is up to the caller. This class just
    //  determines an order for it such that any processor is only talking
    //  to one other at a time. After construction:
    //  - schedule is the order in which comms is done.
    //  - procSchedule[procI] is for procI the order in which comms is done.
    commSchedule(const label nProcs, const List<labelPair>& comms);
  // Member Functions
    //- Order in which comms is scheduled
    const labelList& schedule() const
    {
      return schedule_;
    }
    //- Per processor the order in which communication has been scheduled
    const labelListList& procSchedule() const
    {
      return procSchedule_;
    }
};

}  // namespace mousse

#endif
