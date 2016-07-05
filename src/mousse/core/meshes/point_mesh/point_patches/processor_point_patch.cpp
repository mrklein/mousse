// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_point_patch.hpp"
#include "point_boundary_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "point_mesh.hpp"
#include "face_list.hpp"
#include "primitive_face_patch.hpp"
#include "empty_poly_patch.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(processorPointPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  facePointPatch,
  processorPointPatch,
  polyPatch
);

}


// Protected Member Functions 
void mousse::processorPointPatch::initGeometry(PstreamBuffers&)
{
  // Algorithm:
  // Depending on whether the patch is a master or a slave, get the primitive
  // patch points and filter away the points from the global patch.
  // Create the reversed patch and pick up its points
  // so that the order is correct
  const polyPatch& pp = patch();
  faceList masterFaces(pp.size());
  FOR_ALL(pp, faceI) {
    masterFaces[faceI] = pp[faceI].reverseFace();
  }
  reverseMeshPoints_ = primitiveFacePatch
  (
    masterFaces,
    pp.points()
  ).meshPoints();
}


void mousse::processorPointPatch::calcGeometry(PstreamBuffers&)
{}


void mousse::processorPointPatch::initMovePoints
(
  PstreamBuffers&,
  const pointField&
)
{}


void mousse::processorPointPatch::movePoints(PstreamBuffers&, const pointField&)
{}


void mousse::processorPointPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
  facePointPatch::initUpdateMesh(pBufs);
  processorPointPatch::initGeometry(pBufs);
}


void mousse::processorPointPatch::updateMesh(PstreamBuffers& pBufs)
{
  facePointPatch::updateMesh(pBufs);
  processorPointPatch::calcGeometry(pBufs);
}


// Constructors 
mousse::processorPointPatch::processorPointPatch
(
  const polyPatch& patch,
  const pointBoundaryMesh& bm
)
:
  coupledFacePointPatch{patch, bm},
  procPolyPatch_{refCast<const processorPolyPatch>(patch)}
{}


// Destructor 
mousse::processorPointPatch::~processorPointPatch()
{}


// Member Functions 
const mousse::labelList& mousse::processorPointPatch::reverseMeshPoints() const
{
  return reverseMeshPoints_;
}

