// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetry_point_patch.hpp"
#include "point_constraint.hpp"
#include "add_to_run_time_selection_table.hpp"

namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(symmetryPointPatch, 0);
// Add the patch constructor functions to the hash tables
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  facePointPatch,
  symmetryPointPatch,
  polyPatch
);

}

// Member Functions 
void mousse::symmetryPointPatch::applyConstraint
(
  const label pointi,
  pointConstraint& pc
) const
{
  pc.applyConstraint(pointNormals()[pointi]);
}
