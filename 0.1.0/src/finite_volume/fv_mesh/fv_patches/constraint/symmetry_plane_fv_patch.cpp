// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetry_plane_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(symmetryPlaneFvPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(fvPatch, symmetryPlaneFvPatch, polyPatch);
}  // namespace mousse
