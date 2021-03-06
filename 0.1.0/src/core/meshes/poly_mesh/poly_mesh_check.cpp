// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "poly_mesh_tools.hpp"
#include "unit_conversion.hpp"
#include "sync_tools.hpp"
#include "pstream_reduce_ops.hpp"


// Member Functions
bool mousse::polyMesh::checkFaceOrthogonality
(
  const vectorField& fAreas,
  const vectorField& cellCtrs,
  const bool report,
  const bool detailedReport,
  labelHashSet* setPtr
) const
{
  if (debug) {
    Info << "bool polyMesh::checkFaceOrthogonality("
      << "const bool, labelHashSet*) const: "
      << "checking mesh non-orthogonality" << endl;
  }
  const labelList& own = faceOwner();
  const labelList& nei = faceNeighbour();
  // Calculate orthogonality for all internal and coupled boundary faces
  // (1 for uncoupled boundary faces)
  tmp<scalarField> tortho =
    polyMeshTools::faceOrthogonality
    (
      *this,
      fAreas,
      cellCtrs
    );
  const scalarField& ortho = tortho();
  // Severe nonorthogonality threshold
  const scalar severeNonorthogonalityThreshold =
    ::cos(degToRad(primitiveMesh::nonOrthThreshold_));
  scalar minDDotS = GREAT;
  scalar sumDDotS = 0.0;
  label nSummed = 0;
  label severeNonOrth = 0;
  label errorNonOrth = 0;
  // Statistics only for internal and masters of coupled faces
  PackedBoolList isMasterFace{syncTools::getInternalOrMasterFaces(*this)};
  FOR_ALL(ortho, faceI) {
    if (ortho[faceI] < severeNonorthogonalityThreshold) {
      if (ortho[faceI] > SMALL) {
        if (setPtr) {
          setPtr->insert(faceI);
        }
        severeNonOrth++;
      } else {
        // Error : non-ortho too large
        if (setPtr) {
          setPtr->insert(faceI);
        }
        if (detailedReport && errorNonOrth == 0) {
          // Non-orthogonality greater than 90 deg
          WARNING_IN
          (
            "polyMesh::checkFaceOrthogonality"
            "(const pointField&, const bool) const"
          )
          << "Severe non-orthogonality for face "
          << faceI
          << " between cells " << own[faceI]
          << " and " << nei[faceI]
          << ": Angle = "
          << radToDeg(::acos(min(1.0, max(-1.0, ortho[faceI]))))
          << " deg." << endl;
        }
        errorNonOrth++;
      }
    }
    if (isMasterFace[faceI]) {
      minDDotS = min(minDDotS, ortho[faceI]);
      sumDDotS += ortho[faceI];
      nSummed++;
    }
  }
  reduce(minDDotS, minOp<scalar>());
  reduce(sumDDotS, sumOp<scalar>());
  reduce(nSummed, sumOp<label>());
  reduce(severeNonOrth, sumOp<label>());
  reduce(errorNonOrth, sumOp<label>());
  if (debug || report) {
    if (nSummed > 0) {
      if (debug || report) {
        Info << "    Mesh non-orthogonality Max: "
          << radToDeg(::acos(min(1.0, max(-1.0, minDDotS))))
          << " average: "
          << radToDeg(::acos(min(1.0, max(-1.0, sumDDotS/nSummed))))
          << endl;
      }
    }
    if (severeNonOrth > 0) {
      Info << "   *Number of severely non-orthogonal (> "
        << primitiveMesh::nonOrthThreshold_ << " degrees) faces: "
        << severeNonOrth << "." << endl;
    }
  }
  if (errorNonOrth > 0) {
    if (debug || report) {
      Info << " ***Number of non-orthogonality errors: "
        << errorNonOrth << "." << endl;
    }
    return true;
  } else {
    if (debug || report) {
      Info << "    Non-orthogonality check OK." << endl;
    }
    return false;
  }
}


bool mousse::polyMesh::checkFaceSkewness
(
  const pointField& points,
  const vectorField& fCtrs,
  const vectorField& fAreas,
  const vectorField& cellCtrs,
  const bool report,
  const bool detailedReport,
  labelHashSet* setPtr
) const
{
  if (debug) {
    Info << "bool polyMesh::checkFaceSkewnesss("
      << "const bool, labelHashSet*) const: "
      << "checking face skewness" << endl;
  }
  const labelList& own = faceOwner();
  const labelList& nei = faceNeighbour();
  // Warn if the skew correction vector is more than skewWarning times
  // larger than the face area vector
  tmp<scalarField> tskew =
    polyMeshTools::faceSkewness
    (
      *this,
      points,
      fCtrs,
      fAreas,
      cellCtrs
    );
  const scalarField& skew = tskew();
  scalar maxSkew = max(skew);
  label nWarnSkew = 0;
  // Statistics only for all faces except slave coupled faces
  PackedBoolList isMasterFace{syncTools::getMasterFaces(*this)};
  FOR_ALL(skew, faceI) {
    // Check if the skewness vector is greater than the PN vector.
    // This does not cause trouble but is a good indication of a poor mesh.
    if (skew[faceI] > skewThreshold_) {
      if (setPtr) {
        setPtr->insert(faceI);
      }
      if (detailedReport && nWarnSkew == 0) {
        // Non-orthogonality greater than 90 deg
        if (isInternalFace(faceI)) {
          WARNING_IN
          (
            "polyMesh::checkFaceSkewnesss"
            "(const pointField&, const bool) const"
          )
          << "Severe skewness " << skew[faceI]
          << " for face " << faceI
          << " between cells " << own[faceI]
          << " and " << nei[faceI];
        } else {
          WARNING_IN
          (
            "polyMesh::checkFaceSkewnesss"
            "(const pointField&, const bool) const"
          )
          << "Severe skewness " << skew[faceI]
          << " for boundary face " << faceI
          << " on cell " << own[faceI];
        }
      }
      if (isMasterFace[faceI]) {
        nWarnSkew++;
      }
    }
  }
  reduce(maxSkew, maxOp<scalar>());
  reduce(nWarnSkew, sumOp<label>());
  if (nWarnSkew > 0) {
    if (debug || report) {
      Info << " ***Max skewness = " << maxSkew
        << ", " << nWarnSkew << " highly skew faces detected"
         " which may impair the quality of the results"
        << endl;
    }
    return true;
  } else {
    if (debug || report) {
      Info << "    Max skewness = " << maxSkew << " OK." << endl;
    }
    return false;
  }
}


// Check 1D/2Dness of edges. Gets passed the non-empty directions and
// checks all edges in the mesh whether they:
// - have no component in a non-empty direction or
// - are only in a singe non-empty direction.
// Empty direction info is passed in as a vector of labels (synchronised)
// which are 1 if the direction is non-empty, 0 if it is.
bool mousse::polyMesh::checkEdgeAlignment
(
  const pointField& p,
  const bool report,
  const Vector<label>& directions,
  labelHashSet* setPtr
) const
{
  if (debug) {
    Info << "bool polyMesh::checkEdgeAlignment("
      << "const bool, const Vector<label>&, labelHashSet*) const: "
      << "checking edge alignment" << endl;
  }
  label nDirs = 0;
  for (direction cmpt=0; cmpt<vector::nComponents; cmpt++) {
    if (directions[cmpt] == 1) {
      nDirs++;
    } else if (directions[cmpt] != 0) {
      FATAL_ERROR_IN
      (
        "polyMesh::checkEdgeAlignment"
        "(const bool, const Vector<label>&, labelHashSet*)"
      )
      << "directions should contain 0 or 1 but is now " << directions
      << exit(FatalError);
    }
  }
  if (nDirs == vector::nComponents) {
    return false;
  }
  const faceList& fcs = faces();
  EdgeMap<label> edgesInError;
  FOR_ALL(fcs, faceI) {
    const face& f = fcs[faceI];
    FOR_ALL(f, fp) {
      label p0 = f[fp];
      label p1 = f.nextLabel(fp);
      if (p0 < p1) {
        vector d(p[p1]-p[p0]);
        scalar magD = mag(d);
        if (magD > ROOTVSMALL) {
          d /= magD;
          // Check how many empty directions are used by the edge.
          label nEmptyDirs = 0;
          label nNonEmptyDirs = 0;
          for (direction cmpt=0; cmpt<vector::nComponents; cmpt++) {
            if (mag(d[cmpt]) > 1e-6) {
              if (directions[cmpt] == 0) {
                nEmptyDirs++;
              } else {
                nNonEmptyDirs++;
              }
            }
          }
          if (nEmptyDirs == 0) {
            // Purely in ok directions.
          } else if (nEmptyDirs == 1) {
            // Ok if purely in empty directions.
            if (nNonEmptyDirs > 0) {
              edgesInError.insert(edge(p0, p1), faceI);
            }
          } else if (nEmptyDirs > 1) {
            // Always an error
            edgesInError.insert(edge(p0, p1), faceI);
          }
        }
      }
    }
  }
  label nErrorEdges = returnReduce(edgesInError.size(), sumOp<label>());
  if (nErrorEdges > 0) {
    if (debug || report) {
      Info << " ***Number of edges not aligned with or perpendicular to "
        << "non-empty directions: " << nErrorEdges << endl;
    }
    if (setPtr) {
      setPtr->resize(2*edgesInError.size());
      FOR_ALL_CONST_ITER(EdgeMap<label>, edgesInError, iter) {
        setPtr->insert(iter.key()[0]);
        setPtr->insert(iter.key()[1]);
      }
    }
    return true;
  } else {
    if (debug || report) {
      Info << "    All edges aligned with or perpendicular to "
        << "non-empty directions." << endl;
    }
    return false;
  }
}


bool mousse::polyMesh::checkCellDeterminant
(
  const vectorField& faceAreas,
  const bool report,
  labelHashSet* setPtr,
  const Vector<label>& meshD
) const
{
  const scalar warnDet = 1e-3;
  if (debug) {
    Info << "bool polyMesh::checkCellDeterminant(const bool"
      << ", labelHashSet*) const: "
      << "checking for under-determined cells" << endl;
  }
  tmp<scalarField> tcellDeterminant =
    primitiveMeshTools::cellDeterminant
    (
      *this,
      meshD,
      faceAreas,
      syncTools::getInternalOrCoupledFaces(*this)
    );
  scalarField& cellDeterminant = tcellDeterminant();
  label nErrorCells = 0;
  scalar minDet = min(cellDeterminant);
  scalar sumDet = sum(cellDeterminant);
  FOR_ALL(cellDeterminant, cellI) {
    if (cellDeterminant[cellI] < warnDet) {
      if (setPtr) {
        setPtr->insert(cellI);
      }
      nErrorCells++;
    }
  }
  reduce(nErrorCells, sumOp<label>());
  reduce(minDet, minOp<scalar>());
  reduce(sumDet, sumOp<scalar>());
  label nSummed = returnReduce(cellDeterminant.size(), sumOp<label>());
  if (debug || report) {
    if (nSummed > 0) {
      Info << "    Cell determinant (wellposedness) : minimum: " << minDet
        << " average: " << sumDet/nSummed
        << endl;
    }
  }
  if (nErrorCells > 0) {
    if (debug || report) {
      Info << " ***Cells with small determinant (< "
        << warnDet << ") found, number of cells: "
        << nErrorCells << endl;
    }
    return true;
  } else {
    if (debug || report) {
      Info << "    Cell determinant check OK." << endl;
    }
    return false;
  }
  return false;
}


bool mousse::polyMesh::checkFaceWeight
(
  const vectorField& fCtrs,
  const vectorField& fAreas,
  const vectorField& cellCtrs,
  const bool report,
  const scalar minWeight,
  labelHashSet* setPtr
) const
{
  if (debug) {
    Info << "bool polyMesh::checkFaceWeight(const bool"
      << ", labelHashSet*) const: "
      << "checking for low face interpolation weights" << endl;
  }
  tmp<scalarField> tfaceWght =
    polyMeshTools::faceWeights
    (
      *this,
      fCtrs,
      fAreas,
      cellCtrs
    );
  scalarField& faceWght = tfaceWght();
  label nErrorFaces = 0;
  scalar minDet = GREAT;
  scalar sumDet = 0.0;
  label nSummed = 0;
  // Statistics only for internal and masters of coupled faces
  PackedBoolList isMasterFace{syncTools::getInternalOrMasterFaces(*this)};
  FOR_ALL(faceWght, faceI) {
    if (faceWght[faceI] < minWeight) {
      // Note: insert both sides of coupled faces
      if (setPtr) {
        setPtr->insert(faceI);
      }
      nErrorFaces++;
    }
    // Note: statistics only on master of coupled faces
    if (isMasterFace[faceI]) {
      minDet = min(minDet, faceWght[faceI]);
      sumDet += faceWght[faceI];
      nSummed++;
    }
  }
  reduce(nErrorFaces, sumOp<label>());
  reduce(minDet, minOp<scalar>());
  reduce(sumDet, sumOp<scalar>());
  reduce(nSummed, sumOp<label>());
  if (debug || report) {
    if (nSummed > 0) {
      Info << "    Face interpolation weight : minimum: " << minDet
        << " average: " << sumDet/nSummed
        << endl;
    }
  }
  if (nErrorFaces > 0) {
    if (debug || report) {
      Info << " ***Faces with small interpolation weight (< " << minWeight
        << ") found, number of faces: "
        << nErrorFaces << endl;
    }
    return true;
  } else {
    if (debug || report) {
      Info << "    Face interpolation weight check OK." << endl;
    }
    return false;
  }
  return false;
}


bool mousse::polyMesh::checkVolRatio
(
  const scalarField& cellVols,
  const bool report,
  const scalar minRatio,
  labelHashSet* setPtr
) const
{
  if (debug) {
    Info << "bool polyMesh::checkVolRatio(const bool"
      << ", labelHashSet*) const: "
      << "checking for volume ratio < " << minRatio << endl;
  }
  tmp<scalarField> tvolRatio = polyMeshTools::volRatio(*this, cellVols);
  scalarField& volRatio = tvolRatio();
  label nErrorFaces = 0;
  scalar minDet = GREAT;
  scalar sumDet = 0.0;
  label nSummed = 0;
  // Statistics only for internal and masters of coupled faces
  PackedBoolList isMasterFace{syncTools::getInternalOrMasterFaces(*this)};
  FOR_ALL(volRatio, faceI) {
    if (volRatio[faceI] < minRatio) {
      // Note: insert both sides of coupled faces
      if (setPtr) {
        setPtr->insert(faceI);
      }
      nErrorFaces++;
    }
    // Note: statistics only on master of coupled faces
    if (isMasterFace[faceI]) {
      minDet = min(minDet, volRatio[faceI]);
      sumDet += volRatio[faceI];
      nSummed++;
    }
  }
  reduce(nErrorFaces, sumOp<label>());
  reduce(minDet, minOp<scalar>());
  reduce(sumDet, sumOp<scalar>());
  reduce(nSummed, sumOp<label>());
  if (debug || report) {
    if (nSummed > 0) {
      Info << "    Face volume ratio : minimum: " << minDet
        << " average: " << sumDet/nSummed
        << endl;
    }
  }
  if (nErrorFaces > 0) {
    if (debug || report) {
      Info << " ***Faces with small volume ratio (< " << minRatio
        << ") found, number of faces: "
        << nErrorFaces << endl;
    }
    return true;
  } else {
    if (debug || report) {
      Info << "    Face volume ratio check OK." << endl;
    }
    return false;
  }
  return false;
}


//- Could override checkClosedBoundary to not look at (collocated!) coupled
//  faces
//bool mousse::polyMesh::checkClosedBoundary(const bool report) const
//{
//    return primitiveMesh::checkClosedBoundary
//    (
//        faceAreas(),
//        report,
//        syncTools::getInternalOrCollocatedCoupledFaces(*this)
//    );
//}
bool mousse::polyMesh::checkFaceOrthogonality
(
  const bool report,
  labelHashSet* setPtr
) const
{
  return
    checkFaceOrthogonality
    (
      faceAreas(),
      cellCentres(),
      report,
      false,  // detailedReport
      setPtr
    );
}


bool mousse::polyMesh::checkFaceSkewness
(
  const bool report,
  labelHashSet* setPtr
) const
{
  return
    checkFaceSkewness
    (
      points(),
      faceCentres(),
      faceAreas(),
      cellCentres(),
      report,
      false,  // detailedReport
      setPtr
    );
}


bool mousse::polyMesh::checkEdgeAlignment
(
  const bool report,
  const Vector<label>& directions,
  labelHashSet* setPtr
) const
{
  return
    checkEdgeAlignment
    (
      points(),
      report,
      directions,
      setPtr
    );
}


bool mousse::polyMesh::checkCellDeterminant
(
  const bool report,
  labelHashSet* setPtr
) const
{
  return
    checkCellDeterminant
    (
      faceAreas(),
      report,
      setPtr,
      geometricD()
    );
}


bool mousse::polyMesh::checkFaceWeight
(
  const bool report,
  const scalar minWeight,
  labelHashSet* setPtr
) const
{
  return
    checkFaceWeight
    (
      faceCentres(),
      faceAreas(),
      cellCentres(),
      report,
      minWeight,
      setPtr
    );
}


bool mousse::polyMesh::checkVolRatio
(
  const bool report,
  const scalar minRatio,
  labelHashSet* setPtr
) const
{
  return checkVolRatio(cellVolumes(), report, minRatio, setPtr);
}


bool mousse::polyMesh::checkMeshMotion
(
  const pointField& newPoints,
  const bool report,
  const bool detailedReport
) const
{
  if (debug || report) {
    Pout << "bool polyMesh::checkMeshMotion("
      << "const pointField&, const bool, const bool) const: "
      << "checking mesh motion" << endl;
  }
  vectorField fCtrs{nFaces()};
  vectorField fAreas{nFaces()};
  makeFaceCentresAndAreas(newPoints, fCtrs, fAreas);
  // Check cell volumes and calculate new cell centres
  vectorField cellCtrs{nCells()};
  scalarField cellVols{nCells()};
  makeCellCentresAndVols(fCtrs, fAreas, cellCtrs, cellVols);
  // Check cell volumes
  bool error =
    checkCellVolumes
    (
      cellVols,       // vols
      report,         // report
      detailedReport, // detailedReport
      nullptr         // setPtr
    );
  // Check face areas
  bool areaError =
    checkFaceAreas
    (
      fAreas,
      report,         // report
      detailedReport, // detailedReport,
      nullptr         // setPtr
    );
  error = error || areaError;
  // Check pyramid volumes
  bool pyrVolError =
    checkFacePyramids
    (
      newPoints,
      cellCtrs,
      report,         // report,
      detailedReport, // detailedReport,
      -SMALL,         // minPyrVol
      nullptr         // setPtr
    );
  error = error || pyrVolError;
  // Check face non-orthogonality
  bool nonOrthoError =
    checkFaceOrthogonality
    (
      fAreas,
      cellCtrs,
      report,         // report
      detailedReport, // detailedReport
      nullptr         // setPtr
    );
  error = error || nonOrthoError;
  if (!error && (debug || report)) {
    Pout << "Mesh motion check OK." << endl;
  }
  return error;
}

