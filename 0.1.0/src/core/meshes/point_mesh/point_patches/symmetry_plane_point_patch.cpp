// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetry_plane_point_patch.hpp"
#include "point_constraint.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(symmetryPlanePointPatch, 0);
// Add the patch constructor functions to the hash tables
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  facePointPatch,
  symmetryPlanePointPatch,
  polyPatch
);

}


// Constructors
mousse::symmetryPlanePointPatch::symmetryPlanePointPatch
(
  const polyPatch& patch,
  const pointBoundaryMesh& bm
)
:
  facePointPatch{patch, bm},
  symmetryPlanePolyPatch_{refCast<const symmetryPlanePolyPatch>(patch)}
{}


// Member Functions
void mousse::symmetryPlanePointPatch::applyConstraint
(
  const label,
  pointConstraint& pc
) const
{
  pc.applyConstraint(symmetryPlanePolyPatch_.n());
}
