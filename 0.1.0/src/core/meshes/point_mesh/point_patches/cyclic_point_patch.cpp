// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_point_patch.hpp"
#include "point_boundary_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "point_mesh.hpp"
#include "edge_list.hpp"
// Static Data Members
namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(cyclicPointPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  facePointPatch,
  cyclicPointPatch,
  polyPatch
);

}
// Protected Member Functions 
void mousse::cyclicPointPatch::initGeometry(PstreamBuffers&)
{}
void mousse::cyclicPointPatch::calcGeometry(PstreamBuffers&)
{}
void mousse::cyclicPointPatch::initMovePoints(PstreamBuffers&, const pointField&)
{}
void mousse::cyclicPointPatch::movePoints(PstreamBuffers&, const pointField&)
{}
void mousse::cyclicPointPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
  facePointPatch::initUpdateMesh(pBufs);
  cyclicPointPatch::initGeometry(pBufs);
}
void mousse::cyclicPointPatch::updateMesh(PstreamBuffers& pBufs)
{
  facePointPatch::updateMesh(pBufs);
  cyclicPointPatch::calcGeometry(pBufs);
}
// Constructors 
mousse::cyclicPointPatch::cyclicPointPatch
(
  const polyPatch& patch,
  const pointBoundaryMesh& bm
)
:
  coupledFacePointPatch(patch, bm),
  cyclicPolyPatch_(refCast<const cyclicPolyPatch>(patch))
{}
// Destructor 
mousse::cyclicPointPatch::~cyclicPointPatch()
{}
// Member Functions 
const mousse::edgeList& mousse::cyclicPointPatch::transformPairs() const
{
  return cyclicPolyPatch_.coupledPoints();
}
