// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_coupled_point_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  defineTypeNameAndDebug(regionCoupledPointPatch, 0);
  addToRunTimeSelectionTable
  (
    facePointPatch,
    regionCoupledPointPatch,
    polyPatch
  );
}  // namespace mousse
