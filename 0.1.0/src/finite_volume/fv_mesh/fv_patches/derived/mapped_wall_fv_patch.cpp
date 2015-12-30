// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_wall_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  defineTypeNameAndDebug(mappedWallFvPatch, 0);
  addToRunTimeSelectionTable(fvPatch, mappedWallFvPatch, polyPatch);
}
