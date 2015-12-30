// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_coupled_wall_point_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  defineTypeNameAndDebug(regionCoupledWallPointPatch, 0);
  addToRunTimeSelectionTable
  (
    facePointPatch,
    regionCoupledWallPointPatch,
    polyPatch
  );
}  // namespace mousse
