// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nonuniform_transform_cyclic_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
defineTypeNameAndDebug(nonuniformTransformCyclicFvPatch, 0);
addToRunTimeSelectionTable
(
  fvPatch,
  nonuniformTransformCyclicFvPatch,
  polyPatch
);
}  // namespace mousse
