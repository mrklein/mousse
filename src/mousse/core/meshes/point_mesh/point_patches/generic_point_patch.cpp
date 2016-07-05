// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "generic_point_patch.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(genericPointPatch, 0);
// Add the patch constructor functions to the hash tables
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  facePointPatch,
  genericPointPatch,
  polyPatch
);

}
