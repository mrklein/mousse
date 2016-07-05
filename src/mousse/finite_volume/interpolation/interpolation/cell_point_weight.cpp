// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_point_weight.hpp"
#include "poly_mesh.hpp"
#include "poly_mesh_tet_decomposition.hpp"


// Static Data Members
int mousse::cellPointWeight::debug{debug::debugSwitch("cellPointWeight", 0)};
mousse::scalar mousse::cellPointWeight::tol{SMALL};


// Private Member Functions 
void mousse::cellPointWeight::findTetrahedron
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI
)
{
  if (debug) {
    Pout << nl << "mousse::cellPointWeight::findTetrahedron" << nl
      << "position = " << position << nl
      << "cellI = " << cellI << endl;
  }
  List<tetIndices> cellTets = polyMeshTetDecomposition::cellTetIndices
  (
    mesh,
    cellI
  );
  const faceList& pFaces = mesh.faces();
  const scalar cellVolume = mesh.cellVolumes()[cellI];
  FOR_ALL(cellTets, tetI) {
    const tetIndices& tetIs = cellTets[tetI];
    const face& f = pFaces[tetIs.face()];
    // Barycentric coordinates of the position
    scalar det = tetIs.tet(mesh).barycentric(position, weights_);
    if (mag(det/cellVolume) > tol) {
      const scalar& u = weights_[0];
      const scalar& v = weights_[1];
      const scalar& w = weights_[2];
      if ((u + tol > 0)
          && (v + tol > 0)
          && (w + tol > 0)
          && (u + v + w < 1 + tol)) {
        faceVertices_[0] = f[tetIs.faceBasePt()];
        faceVertices_[1] = f[tetIs.facePtA()];
        faceVertices_[2] = f[tetIs.facePtB()];
        return;
      }
    }
  }
  // A suitable point in a tetrahedron was not found, find the
  // nearest.
  scalar minNearDist = VGREAT;
  label nearestTetI = -1;
  FOR_ALL(cellTets, tetI) {
    const tetIndices& tetIs = cellTets[tetI];
    scalar nearDist = tetIs.tet(mesh).nearestPoint(position).distance();
    if (nearDist < minNearDist) {
      minNearDist = nearDist;
      nearestTetI = tetI;
    }
  }
  if (debug) {
    Pout << "cellPointWeight::findTetrahedron" << nl
      << "    Tetrahedron search failed; using closest tet to point "
      << position << nl
      << "    cell: "
      << cellI << nl
      << endl;
  }
  const tetIndices& tetIs = cellTets[nearestTetI];
  const face& f = pFaces[tetIs.face()];
  // Barycentric coordinates of the position, ignoring if the
  // determinant is suitable.  If not, the return from barycentric
  // to weights_ is safe.
  tetIs.tet(mesh).barycentric(position, weights_);
  faceVertices_[0] = f[tetIs.faceBasePt()];
  faceVertices_[1] = f[tetIs.facePtA()];
  faceVertices_[2] = f[tetIs.facePtB()];
}


void mousse::cellPointWeight::findTriangle
(
  const polyMesh& mesh,
  const vector& position,
  const label faceI
)
{
  if (debug) {
    Pout << "\nbool mousse::cellPointWeight::findTriangle" << nl
      << "position = " << position << nl
      << "faceI = " << faceI << endl;
  }
  List<tetIndices> faceTets = polyMeshTetDecomposition::faceTetIndices
  (
    mesh,
    faceI,
    mesh.faceOwner()[faceI]
  );
  const scalar faceAreaSqr = magSqr(mesh.faceAreas()[faceI]);
  const face& f =  mesh.faces()[faceI];
  FOR_ALL(faceTets, tetI) {
    const tetIndices& tetIs = faceTets[tetI];
    List<scalar> triWeights{3};
    // Barycentric coordinates of the position
    scalar det = tetIs.faceTri(mesh).barycentric(position, triWeights);
    if (0.25*mag(det)/faceAreaSqr > tol) {
      const scalar& u = triWeights[0];
      const scalar& v = triWeights[1];
      if ((u + tol > 0)
          && (v + tol > 0)
          && (u + v < 1 + tol)) {
        // Weight[0] is for the cell centre.
        weights_[0] = 0;
        weights_[1] = triWeights[0];
        weights_[2] = triWeights[1];
        weights_[3] = triWeights[2];
        faceVertices_[0] = f[tetIs.faceBasePt()];
        faceVertices_[1] = f[tetIs.facePtA()];
        faceVertices_[2] = f[tetIs.facePtB()];
        return;
      }
    }
  }
  // A suitable point in a triangle was not found, find the nearest.
  scalar minNearDist = VGREAT;
  label nearestTetI = -1;
  FOR_ALL(faceTets, tetI) {
    const tetIndices& tetIs = faceTets[tetI];
    scalar nearDist = tetIs.faceTri(mesh).nearestPoint(position).distance();
    if (nearDist < minNearDist) {
      minNearDist = nearDist;
      nearestTetI = tetI;
    }
  }
  if (debug) {
    Pout << "cellPointWeight::findTriangle" << nl
      << "    Triangle search failed; using closest tri to point "
      << position << nl
      << "    face: "
      << faceI << nl
      << endl;
  }
  const tetIndices& tetIs = faceTets[nearestTetI];
  // Barycentric coordinates of the position, ignoring if the
  // determinant is suitable.  If not, the return from barycentric
  // to triWeights is safe.
  List<scalar> triWeights{3};
  tetIs.faceTri(mesh).barycentric(position, triWeights);
  // Weight[0] is for the cell centre.
  weights_[0] = 0;
  weights_[1] = triWeights[0];
  weights_[2] = triWeights[1];
  weights_[3] = triWeights[2];
  faceVertices_[0] = f[tetIs.faceBasePt()];
  faceVertices_[1] = f[tetIs.facePtA()];
  faceVertices_[2] = f[tetIs.facePtB()];
}


// Constructors 
mousse::cellPointWeight::cellPointWeight
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label faceI
)
:
  cellI_{cellI},
  weights_{4},
  faceVertices_{3}
{
  if (faceI < 0) {
    // Face data not supplied
    findTetrahedron(mesh, position, cellI);
  } else {
    // Face data supplied
    findTriangle(mesh, position, faceI);
  }
}

