// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_point_weight_wall_modified.hpp"


// Constructors 
mousse::cellPointWeightWallModified::cellPointWeightWallModified
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label faceI
)
:
  cellPointWeight{mesh, position, cellI, faceI}
{
  // findTetrahedron or findTriangle will already have been called
  // by the cellPointWeight constructor
  if (faceI >= 0) {
    const polyBoundaryMesh& bm = mesh.boundaryMesh();
    label patchI = bm.whichPatch(faceI);
    if (patchI != -1) {
      if (isA<wallPolyPatch>(bm[patchI])) {
        // Apply cell centre value wall faces
        weights_[0] = 1.0;
        weights_[1] = 0.0;
        weights_[2] = 0.0;
        weights_[3] = 0.0;
      }
    }
  }
}
