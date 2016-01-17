// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wedge_point_patch.hpp"
#include "point_constraint.hpp"
#include "add_to_run_time_selection_table.hpp"

namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(wedgePointPatch, 0);
// Add the patch constructor functions to the hash tables
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  facePointPatch,
  wedgePointPatch,
  polyPatch
);

}
// Constructors 
mousse::wedgePointPatch::wedgePointPatch
(
  const polyPatch& patch,
  const pointBoundaryMesh& bm
)
:
  facePointPatch{patch, bm},
  wedgePolyPatch_{refCast<const wedgePolyPatch>(patch)}
{}
// Member Functions 
void mousse::wedgePointPatch::applyConstraint
(
  const label /*pointi*/,
  pointConstraint& pc
) const
{
  pc.applyConstraint(wedgePolyPatch_.n());
}
