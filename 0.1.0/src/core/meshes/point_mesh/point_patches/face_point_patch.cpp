// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_point_patch.hpp"
#include "point_boundary_mesh.hpp"
#include "point_mesh.hpp"
#include "demand_driven_data.hpp"
#include "bool_list.hpp"
#include "add_to_run_time_selection_table.hpp"

namespace mousse
{

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(facePointPatch, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(facePointPatch, polyPatch);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  facePointPatch,
  facePointPatch,
  polyPatch
);

// Protected Member Functions
void facePointPatch::initGeometry(PstreamBuffers&)
{}
void facePointPatch::calcGeometry(PstreamBuffers&)
{}
void facePointPatch::initMovePoints(PstreamBuffers&, const pointField&)
{}
void facePointPatch::movePoints(PstreamBuffers&, const pointField&)
{}
void facePointPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
  facePointPatch::initGeometry(pBufs);
}
void facePointPatch::updateMesh(PstreamBuffers&)
{}

// Constructors
facePointPatch::facePointPatch
(
  const polyPatch& p,
  const pointBoundaryMesh& bm
)
:
  pointPatch{bm},
  polyPatch_{p}
{}
}  // namespace mousse
