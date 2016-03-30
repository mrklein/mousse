// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "none_gamg_proc_agglomeration.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "gamg_agglomeration.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(noneGAMGProcAgglomeration, 0);

ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGProcAgglomeration,
  noneGAMGProcAgglomeration,
  GAMGAgglomeration,
  none
);

}


// Private Member Functions
// Constructors
mousse::noneGAMGProcAgglomeration::noneGAMGProcAgglomeration
(
  GAMGAgglomeration& agglom,
  const dictionary& controlDict
)
:
  GAMGProcAgglomeration{agglom, controlDict}
{}


// Destructor
mousse::noneGAMGProcAgglomeration::~noneGAMGProcAgglomeration()
{}
