// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coupled_face_point_patch.hpp"
#include "point_boundary_mesh.hpp"

// Static Data Members
namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(coupledFacePointPatch, 0);

}
// Constructors
mousse::coupledFacePointPatch::coupledFacePointPatch
(
  const polyPatch& patch,
  const pointBoundaryMesh& bm
)
:
  facePointPatch{patch, bm},
  coupledPointPatch{bm},
  coupledPolyPatch_{refCast<const coupledPolyPatch>(patch)}
{}

// Destructor
mousse::coupledFacePointPatch::~coupledFacePointPatch()
{}
