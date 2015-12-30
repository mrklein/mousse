// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_ami_point_patch.hpp"
#include "point_boundary_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(cyclicAMIPointPatch, 0);
  addToRunTimeSelectionTable
  (
    facePointPatch,
    cyclicAMIPointPatch,
    polyPatch
  );
}
// Protected Member Functions 
void mousse::cyclicAMIPointPatch::initGeometry(PstreamBuffers&)
{}
void mousse::cyclicAMIPointPatch::calcGeometry(PstreamBuffers&)
{}
void mousse::cyclicAMIPointPatch::initMovePoints
(
  PstreamBuffers&,
  const pointField&
)
{}
void mousse::cyclicAMIPointPatch::movePoints(PstreamBuffers&, const pointField&)
{}
void mousse::cyclicAMIPointPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
  facePointPatch::initUpdateMesh(pBufs);
//    cyclicAMIPointPatch::initGeometry(pBufs);
}
void mousse::cyclicAMIPointPatch::updateMesh(PstreamBuffers& pBufs)
{
  facePointPatch::updateMesh(pBufs);
//    cyclicAMIPointPatch::calcGeometry(pBufs);
}
// Constructors 
mousse::cyclicAMIPointPatch::cyclicAMIPointPatch
(
  const polyPatch& patch,
  const pointBoundaryMesh& bm
)
:
  coupledFacePointPatch(patch, bm),
  cyclicAMIPolyPatch_(refCast<const cyclicAMIPolyPatch>(patch))
{}
// Destructor 
mousse::cyclicAMIPointPatch::~cyclicAMIPointPatch()
{}
