// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_wall_point_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
defineTypeNameAndDebug(mappedWallPointPatch, 0);
// Add the patch constructor functions to the hash tables
addToRunTimeSelectionTable
(
  facePointPatch,
  mappedWallPointPatch,
  polyPatch
);
}  // namespace mousse
