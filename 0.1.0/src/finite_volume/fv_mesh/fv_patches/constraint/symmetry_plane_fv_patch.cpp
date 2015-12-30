// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetry_plane_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
defineTypeNameAndDebug(symmetryPlaneFvPatch, 0);
addToRunTimeSelectionTable(fvPatch, symmetryPlaneFvPatch, polyPatch);
}  // namespace mousse
