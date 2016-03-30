// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh_tools.hpp"
#include "sync_tools.hpp"
#include "pyramid_point_face_ref.hpp"


// Private Member Functions 
mousse::scalar mousse::primitiveMeshTools::faceSkewness
(
  const primitiveMesh& mesh,
  const pointField& p,
  const vectorField& fCtrs,
  const vectorField& fAreas,
  const label faceI,
  const point& ownCc,
  const point& neiCc
)
{
  vector Cpf = fCtrs[faceI] - ownCc;
  vector d = neiCc - ownCc;
  // Skewness vector
  vector sv = Cpf
    - ((fAreas[faceI] & Cpf)/((fAreas[faceI] & d) + ROOTVSMALL))*d;
  vector svHat = sv/(mag(sv) + ROOTVSMALL);
  // Normalisation distance calculated as the approximate distance
  // from the face centre to the edge of the face in the direction
  // of the skewness
  scalar fd = 0.2*mag(d) + ROOTVSMALL;
  const face& f = mesh.faces()[faceI];
  FOR_ALL(f, pi) {
    fd = max(fd, mag(svHat & (p[f[pi]] - fCtrs[faceI])));
  }
  // Normalised skewness
  return mag(sv)/fd;
}


mousse::scalar mousse::primitiveMeshTools::boundaryFaceSkewness
(
  const primitiveMesh& mesh,
  const pointField& p,
  const vectorField& fCtrs,
  const vectorField& fAreas,
  const label faceI,
  const point& ownCc
)
{
  vector Cpf = fCtrs[faceI] - ownCc;
  vector normal = fAreas[faceI];
  normal /= mag(normal) + ROOTVSMALL;
  vector d = normal*(normal & Cpf);
  // Skewness vector
  vector sv = Cpf
    - ((fAreas[faceI] & Cpf)/((fAreas[faceI] & d) + ROOTVSMALL))*d;
  vector svHat = sv/(mag(sv) + ROOTVSMALL);
  // Normalisation distance calculated as the approximate distance
  // from the face centre to the edge of the face in the direction
  // of the skewness
  scalar fd = 0.4*mag(d) + ROOTVSMALL;
  const face& f = mesh.faces()[faceI];
  FOR_ALL(f, pi) {
    fd = max(fd, mag(svHat & (p[f[pi]] - fCtrs[faceI])));
  }
  // Normalised skewness
  return mag(sv)/fd;
}


mousse::scalar mousse::primitiveMeshTools::faceOrthogonality
(
  const point& ownCc,
  const point& neiCc,
  const vector& s
)
{
  vector d = neiCc - ownCc;
  return (d & s)/(mag(d)*mag(s) + ROOTVSMALL);
}


// Member Functions 
mousse::tmp<mousse::scalarField> mousse::primitiveMeshTools::faceOrthogonality
(
  const primitiveMesh& mesh,
  const vectorField& areas,
  const vectorField& cc
)
{
  const labelList& own = mesh.faceOwner();
  const labelList& nei = mesh.faceNeighbour();
  tmp<scalarField> tortho{new scalarField{mesh.nInternalFaces()}};
  scalarField& ortho = tortho();
  // Internal faces
  FOR_ALL(nei, faceI) {
    ortho[faceI] = faceOrthogonality
    (
      cc[own[faceI]],
      cc[nei[faceI]],
      areas[faceI]
    );
  }
  return tortho;
}


mousse::tmp<mousse::scalarField> mousse::primitiveMeshTools::faceSkewness
(
  const primitiveMesh& mesh,
  const pointField& p,
  const vectorField& fCtrs,
  const vectorField& fAreas,
  const vectorField& cellCtrs
)
{
  const labelList& own = mesh.faceOwner();
  const labelList& nei = mesh.faceNeighbour();
  tmp<scalarField> tskew{new scalarField{mesh.nFaces()}};
  scalarField& skew = tskew();
  FOR_ALL(nei, faceI) {
    skew[faceI] = faceSkewness
    (
      mesh,
      p,
      fCtrs,
      fAreas,
      faceI,
      cellCtrs[own[faceI]],
      cellCtrs[nei[faceI]]
    );
  }
  // Boundary faces: consider them to have only skewness error.
  // (i.e. treat as if mirror cell on other side)
  for (label faceI = mesh.nInternalFaces(); faceI < mesh.nFaces(); faceI++) {
    skew[faceI] = boundaryFaceSkewness
    (
      mesh,
      p,
      fCtrs,
      fAreas,
      faceI,
      cellCtrs[own[faceI]]
    );
  }
  return tskew;
}


void mousse::primitiveMeshTools::facePyramidVolume
(
  const primitiveMesh& mesh,
  const pointField& points,
  const vectorField& ctrs,
  scalarField& ownPyrVol,
  scalarField& neiPyrVol
)
{
  const labelList& own = mesh.faceOwner();
  const labelList& nei = mesh.faceNeighbour();
  const faceList& f = mesh.faces();
  ownPyrVol.setSize(mesh.nFaces());
  neiPyrVol.setSize(mesh.nInternalFaces());
  FOR_ALL(f, faceI) {
    // Create the owner pyramid
    ownPyrVol[faceI] = -pyramidPointFaceRef
    (
      f[faceI],
      ctrs[own[faceI]]
    ).mag(points);
    if (mesh.isInternalFace(faceI)) {
      // Create the neighbour pyramid - it will have positive volume
      neiPyrVol[faceI] = pyramidPointFaceRef
      (
        f[faceI],
        ctrs[nei[faceI]]
      ).mag(points);
    }
  }
}


void mousse::primitiveMeshTools::cellClosedness
(
  const primitiveMesh& mesh,
  const Vector<label>& meshD,
  const vectorField& areas,
  const scalarField& vols,
  scalarField& openness,
  scalarField& aratio
)
{
  const labelList& own = mesh.faceOwner();
  const labelList& nei = mesh.faceNeighbour();
  // Loop through cell faces and sum up the face area vectors for each cell.
  // This should be zero in all vector components
  vectorField sumClosed{mesh.nCells(), vector::zero};
  vectorField sumMagClosed{mesh.nCells(), vector::zero};
  FOR_ALL(own, faceI) {
    // Add to owner
    sumClosed[own[faceI]] += areas[faceI];
    sumMagClosed[own[faceI]] += cmptMag(areas[faceI]);
  }
  FOR_ALL(nei, faceI) {
    // Subtract from neighbour
    sumClosed[nei[faceI]] -= areas[faceI];
    sumMagClosed[nei[faceI]] += cmptMag(areas[faceI]);
  }
  label nDims = 0;
  for (direction dir = 0; dir < vector::nComponents; dir++) {
    if (meshD[dir] == 1) {
      nDims++;
    }
  }
  // Check the sums
  openness.setSize(mesh.nCells());
  aratio.setSize(mesh.nCells());
  FOR_ALL(sumClosed, cellI) {
    scalar maxOpenness = 0;
    for (direction cmpt=0; cmpt<vector::nComponents; cmpt++) {
      maxOpenness = max
      (
        maxOpenness,
        mag(sumClosed[cellI][cmpt])
       /(sumMagClosed[cellI][cmpt] + ROOTVSMALL)
      );
    }
    openness[cellI] = maxOpenness;
    // Calculate the aspect ration as the maximum of Cartesian component
    // aspect ratio to the total area hydraulic area aspect ratio
    scalar minCmpt = VGREAT;
    scalar maxCmpt = -VGREAT;
    for (direction dir = 0; dir < vector::nComponents; dir++) {
      if (meshD[dir] == 1) {
        minCmpt = min(minCmpt, sumMagClosed[cellI][dir]);
        maxCmpt = max(maxCmpt, sumMagClosed[cellI][dir]);
      }
    }
    scalar aspectRatio = maxCmpt/(minCmpt + ROOTVSMALL);
    if (nDims == 3) {
      scalar v = max(ROOTVSMALL, vols[cellI]);
      aspectRatio = max
      (
        aspectRatio,
        1.0/6.0*cmptSum(sumMagClosed[cellI])/pow(v, 2.0/3.0)
      );
    }
    aratio[cellI] = aspectRatio;
  }
}


mousse::tmp<mousse::scalarField> mousse::primitiveMeshTools::faceConcavity
(
  const scalar maxSin,
  const primitiveMesh& mesh,
  const pointField& p,
  const vectorField& faceAreas
)
{
  const faceList& fcs = mesh.faces();
  vectorField faceNormals{faceAreas};
  faceNormals /= mag(faceNormals) + ROOTVSMALL;
  tmp<scalarField> tfaceAngles{new scalarField{mesh.nFaces()}};
  scalarField& faceAngles = tfaceAngles();
  FOR_ALL(fcs, faceI) {
    const face& f = fcs[faceI];
    // Get edge from f[0] to f[size-1];
    vector ePrev(p[f.first()] - p[f.last()]);
    scalar magEPrev = mag(ePrev);
    ePrev /= magEPrev + ROOTVSMALL;
    scalar maxEdgeSin = 0.0;
    FOR_ALL(f, fp0) {
      // Get vertex after fp
      label fp1 = f.fcIndex(fp0);
      // Normalized vector between two consecutive points
      vector e10(p[f[fp1]] - p[f[fp0]]);
      scalar magE10 = mag(e10);
      e10 /= magE10 + ROOTVSMALL;
      if (magEPrev > SMALL && magE10 > SMALL) {
        vector edgeNormal = ePrev ^ e10;
        scalar magEdgeNormal = mag(edgeNormal);
        if (magEdgeNormal < maxSin) {
          // Edges (almost) aligned -> face is ok.
        } else {
          // Check normal
          edgeNormal /= magEdgeNormal;
          if ((edgeNormal & faceNormals[faceI]) < SMALL) {
            maxEdgeSin = max(maxEdgeSin, magEdgeNormal);
          }
        }
      }
      ePrev = e10;
      magEPrev = magE10;
    }
    faceAngles[faceI] = maxEdgeSin;
  }
  return tfaceAngles;
}


mousse::tmp<mousse::scalarField> mousse::primitiveMeshTools::faceFlatness
(
  const primitiveMesh& mesh,
  const pointField& p,
  const vectorField& fCtrs,
  const vectorField& faceAreas
)
{
  const faceList& fcs = mesh.faces();
  // Areas are calculated as the sum of areas. (see
  // primitiveMeshFaceCentresAndAreas.C)
  scalarField magAreas{mag(faceAreas)};
  tmp<scalarField> tfaceFlatness{new scalarField{mesh.nFaces(), 1.0}};
  scalarField& faceFlatness = tfaceFlatness();
  FOR_ALL(fcs, faceI) {
    const face& f = fcs[faceI];
    if (f.size() > 3 && magAreas[faceI] > ROOTVSMALL) {
      const point& fc = fCtrs[faceI];
      // Calculate the sum of magnitude of areas and compare to magnitude
      // of sum of areas.
      scalar sumA = 0.0;
      FOR_ALL(f, fp) {
        const point& thisPoint = p[f[fp]];
        const point& nextPoint = p[f.nextLabel(fp)];
        // Triangle around fc.
        vector n = 0.5*((nextPoint - thisPoint)^(fc - thisPoint));
        sumA += mag(n);
      }
      faceFlatness[faceI] = magAreas[faceI]/(sumA + ROOTVSMALL);
    }
  }
  return tfaceFlatness;
}


mousse::tmp<mousse::scalarField> mousse::primitiveMeshTools::cellDeterminant
(
  const primitiveMesh& mesh,
  const Vector<label>& meshD,
  const vectorField& faceAreas,
  const PackedBoolList& internalOrCoupledFace
)
{
  // Determine number of dimensions and (for 2D) missing dimension
  label nDims = 0;
  label twoD = -1;
  for (direction dir = 0; dir < vector::nComponents; dir++) {
    if (meshD[dir] == 1) {
      nDims++;
    } else {
      twoD = dir;
    }
  }
  tmp<scalarField> tcellDeterminant{new scalarField{mesh.nCells()}};
  scalarField& cellDeterminant = tcellDeterminant();
  const cellList& c = mesh.cells();
  if (nDims == 1) {
    cellDeterminant = 1.0;
  } else {
    FOR_ALL(c, cellI) {
      const labelList& curFaces = c[cellI];
      // Calculate local normalization factor
      scalar avgArea = 0;
      label nInternalFaces = 0;
      FOR_ALL(curFaces, i) {
        if (internalOrCoupledFace[curFaces[i]]) {
          avgArea += mag(faceAreas[curFaces[i]]);
          nInternalFaces++;
        }
      }
      if (nInternalFaces == 0) {
        cellDeterminant[cellI] = 0;
      } else {
        avgArea /= nInternalFaces;
        symmTensor areaTensor{symmTensor::zero};
        FOR_ALL(curFaces, i) {
          if (internalOrCoupledFace[curFaces[i]]) {
            areaTensor += sqr(faceAreas[curFaces[i]]/avgArea);
          }
        }
        if (nDims == 2) {
          // Add the missing eigenvector (such that it does not
          // affect the determinant)
          if (twoD == 0) {
            areaTensor.xx() = 1;
          } else if (twoD == 1) {
            areaTensor.yy() = 1;
          } else {
            areaTensor.zz() = 1;
          }
        }
        cellDeterminant[cellI] = mag(det(areaTensor));
      }
    }
  }
  return tcellDeterminant;
}

