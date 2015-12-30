// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_acmi_point_patch.hpp"
#include "point_boundary_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(cyclicACMIPointPatch, 0);
  addToRunTimeSelectionTable
  (
    facePointPatch,
    cyclicACMIPointPatch,
    polyPatch
  );
}
// Protected Member Functions 
void mousse::cyclicACMIPointPatch::initGeometry(PstreamBuffers&)
{}
void mousse::cyclicACMIPointPatch::calcGeometry(PstreamBuffers&)
{}
void mousse::cyclicACMIPointPatch::initMovePoints
(
  PstreamBuffers&,
  const pointField&
)
{}
void mousse::cyclicACMIPointPatch::movePoints(PstreamBuffers&, const pointField&)
{}
void mousse::cyclicACMIPointPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
  facePointPatch::initUpdateMesh(pBufs);
//    cyclicACMIPointPatch::initGeometry(pBufs);
}
void mousse::cyclicACMIPointPatch::updateMesh(PstreamBuffers& pBufs)
{
  facePointPatch::updateMesh(pBufs);
//    cyclicACMIPointPatch::calcGeometry(pBufs);
}
// Constructors 
mousse::cyclicACMIPointPatch::cyclicACMIPointPatch
(
  const polyPatch& patch,
  const pointBoundaryMesh& bm
)
:
  coupledFacePointPatch(patch, bm),
  cyclicACMIPolyPatch_(refCast<const cyclicACMIPolyPatch>(patch))
{}
// Destructor 
mousse::cyclicACMIPointPatch::~cyclicACMIPointPatch()
{}
