// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_point_patch.hpp"


mousse::autoPtr<mousse::facePointPatch> mousse::facePointPatch::New
(
  const polyPatch& patch,
  const pointBoundaryMesh& bm
)
{
  if (debug) {
    Info
      << "facePointPatch::New(const polyPatch&, "
      << " const pointBoundaryMesh&) : "
      << "constructing facePointPatch"
      << endl;
  }
  polyPatchConstructorTable::iterator cstrIter =
    polyPatchConstructorTablePtr_->find(patch.type());
  if (cstrIter == polyPatchConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "facePointPatch::New(const polyPatch&, "
      "const pointBoundaryMesh&) : "
    )
    << "Unknown facePointPatch type "
    << patch.type()
    << nl << nl
    << "Valid facePointPatch types are :" << endl
    << polyPatchConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<facePointPatch>{cstrIter()(patch, bm)};
}
