// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_cyclic_gamg_interface_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ldu_matrix.hpp"

// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(processorCyclicGAMGInterfaceField, 0);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  processorCyclicGAMGInterfaceField,
  lduInterface
);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  processorCyclicGAMGInterfaceField,
  lduInterfaceField
);

}

// Constructors
mousse::processorCyclicGAMGInterfaceField::processorCyclicGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const lduInterfaceField& fineInterface
)
:
  processorGAMGInterfaceField(GAMGCp, fineInterface)
{}
mousse::processorCyclicGAMGInterfaceField::processorCyclicGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const bool doTransform,
  const int rank
)
:
  processorGAMGInterfaceField(GAMGCp, doTransform, rank)
{}

// Destructor
mousse::processorCyclicGAMGInterfaceField::~processorCyclicGAMGInterfaceField()
{}
