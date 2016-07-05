// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_cyclic_gamg_interface.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "map.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(processorCyclicGAMGInterface, 0);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterface,
  processorCyclicGAMGInterface,
  lduInterface
);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterface,
  processorCyclicGAMGInterface,
  Istream
);

}


// Constructors
mousse::processorCyclicGAMGInterface::processorCyclicGAMGInterface
(
  const label index,
  const lduInterfacePtrsList& coarseInterfaces,
  const lduInterface& fineInterface,
  const labelField& localRestrictAddressing,
  const labelField& neighbourRestrictAddressing,
  const label fineLevelIndex,
  const label coarseComm
)
:
  processorGAMGInterface
  {
    index,
    coarseInterfaces,
    fineInterface,
    localRestrictAddressing,
    neighbourRestrictAddressing,
    fineLevelIndex,
    coarseComm
  }
{}


mousse::processorCyclicGAMGInterface::processorCyclicGAMGInterface
(
  const label index,
  const lduInterfacePtrsList& coarseInterfaces,
  Istream& is
)
:
  processorGAMGInterface{index, coarseInterfaces, is}
{}


// Desstructor
mousse::processorCyclicGAMGInterface::~processorCyclicGAMGInterface()
{}

