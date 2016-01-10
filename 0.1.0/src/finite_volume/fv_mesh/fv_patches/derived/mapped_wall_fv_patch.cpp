// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_wall_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "map_distribute.hpp"

namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(mappedWallFvPatch, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(fvPatch, mappedWallFvPatch, polyPatch);
}
