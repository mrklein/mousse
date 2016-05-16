// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "background_mesh_decomposition.hpp"
#include "point_conversion.hpp"


// Member Functions 
template<typename PointType>
mousse::autoPtr<mousse::mapDistribute>
mousse::backgroundMeshDecomposition::distributePoints
(
  List<PointType>& points
) const
{
  labelList toProc{processorPosition(points)};
  autoPtr<mapDistribute> map{buildMap(toProc)};
  map().distribute(points);
  return map;
}


template<typename PointType>
mousse::labelList mousse::backgroundMeshDecomposition::processorPosition
(
  const List<PointType>& pts
) const
{
  DynamicList<label> toCandidateProc;
  DynamicList<point> testPoints;
  labelList ptBlockStart{pts.size(), -1};
  labelList ptBlockSize{pts.size(), -1};
  label nTotalCandidates = 0;
  FOR_ALL(pts, pI) {
    pointFromPoint pt = topoint(pts[pI]);
    label nCandidates = 0;
    FOR_ALL(allBackgroundMeshBounds_, procI) {
      if (allBackgroundMeshBounds_[procI].contains(pt)) {
        toCandidateProc.append(procI);
        testPoints.append(pt);
        nCandidates++;
      }
    }
    ptBlockStart[pI] = nTotalCandidates;
    ptBlockSize[pI] = nCandidates;
    nTotalCandidates += nCandidates;
  }
  // Needed for reverseDistribute
  label preDistributionToCandidateProcSize = toCandidateProc.size();
  autoPtr<mapDistribute> map{buildMap(toCandidateProc)};
  map().distribute(testPoints);
  List<bool> pointOnCandidate{testPoints.size(), false};
  // Test candidate points on candidate processors
  FOR_ALL(testPoints, tPI) {
    pointOnCandidate[tPI] = positionOnThisProcessor(testPoints[tPI]);
  }
  map().reverseDistribute
  (
    preDistributionToCandidateProcSize,
    pointOnCandidate
  );
  labelList ptProc{pts.size(), -1};
  DynamicList<label> failedPointIndices;
  DynamicList<point> failedPoints;
  FOR_ALL(pts, pI) {
    // Extract the sub list of results for this point
    SubList<bool> ptProcResults
    {
      pointOnCandidate,
      ptBlockSize[pI],
      ptBlockStart[pI]
    };
    FOR_ALL(ptProcResults, pPRI) {
      if (ptProcResults[pPRI]) {
        ptProc[pI] = toCandidateProc[ptBlockStart[pI] + pPRI];
        break;
      }
    }
    if (ptProc[pI] < 0) {
      pointFromPoint pt = topoint(pts[pI]);
      if (!globalBackgroundBounds_.contains(pt)) {
        FATAL_ERROR_IN
        (
          "mousse::labelList"
          "mousse::backgroundMeshDecomposition::processorPosition"
          "("
          "  const List<point>&"
          ") const"
        )
        << "The position " << pt
        << " is not in any part of the background mesh "
        << globalBackgroundBounds_ << endl
        << "A background mesh with a wider margin around "
        << "the geometry may help."
        << exit(FatalError);
      }
      if (debug) {
        WARNING_IN
        (
          "mousse::labelList"
          "mousse::backgroundMeshDecomposition::processorPosition"
          "("
          "  const List<point>&"
          ") const"
        )
        << "The position " << pt
        << " was not found in the background mesh "
        << globalBackgroundBounds_ << ", finding nearest."
        << endl;
      }
      failedPointIndices.append(pI);
      failedPoints.append(pt);
    }
  }
  labelList ptNearestProc{processorNearestPosition(failedPoints)};
  FOR_ALL(failedPoints, fPI) {
    label pI = failedPointIndices[fPI];
    ptProc[pI] = ptNearestProc[fPI];
  }
  return ptProc;
}

