// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sliding_interface.hpp"
#include "poly_mesh.hpp"
#include "line.hpp"
#include "poly_topo_changer.hpp"


// Static Data Members
const mousse::scalar mousse::slidingInterface::pointMergeTolDefault_ = 0.05;
const mousse::scalar mousse::slidingInterface::edgeMergeTolDefault_ = 0.01;
const mousse::label mousse::slidingInterface::nFacesPerSlaveEdgeDefault_ = 5;
const mousse::label mousse::slidingInterface::edgeFaceEscapeLimitDefault_ = 10;
const mousse::scalar mousse::slidingInterface::integralAdjTolDefault_ = 0.05;
const mousse::scalar
  mousse::slidingInterface::edgeMasterCatchFractionDefault_ = 0.4;
const mousse::scalar mousse::slidingInterface::edgeEndCutoffTolDefault_ = 0.0001;


// Private Member Functions 

// Index of debug signs:
// a - integral match adjustment: point adjusted
// n - integral match adjustment: point not adjusted
// . - loop of the edge-to-face interaction detection
// x - reversal of direction in edge-to-face interaction detection
// + - complete edge-to-face interaction detection
// z - incomplete edge-to-face interaction detection.  This may be OK for edges
//     crossing from one to the other side of multiply connected master patch
// * - colinear triangle: adjusting projection with slave face normal
// m - master point inserted into the edge
bool mousse::slidingInterface::projectPoints() const
{
  if (debug) {
    Pout << "bool slidingInterface::projectPoints() : "
      << " for object " << name() << " : "
      << "Projecting slave points onto master surface." << endl;
  }
  // Algorithm:
  // 1) Go through all the points of the master and slave patch and calculate
  //    minimum edge length coming from the point.  Calculate the point
  //    merge tolerance as the fraction of mimimum edge length.
  // 2) Project all the slave points onto the master patch
  //    in the normal direction.
  // 3) If some points have missed and the match is integral, the
  //    points in question will be adjusted.  Find the nearest face for
  //    those points and continue with the procedure.
  // 4) For every point, check all the points of the face it has hit.
  //    For every pair of points find if their distance is smaller than
  //    both the master and slave merge tolerance.  If so, the slave point
  //    is moved to the location of the master point.  Remember the master
  //    point index.
  // 5) For every unmerged slave point, check its distance to all the
  //    edges of the face it has hit.  If the distance is smaller than the
  //    edge merge tolerance, the point will be moved onto the edge.
  //    Remember the master edge index.
  // 6) The remaning slave points will be projected into faces.  Remember the
  //    master face index.
  // 7) For the points that miss the master patch, grab the nearest face
  //    on the master and leave the slave point where it started
  //    from and the miss is recorded.
  const polyMesh& mesh = topoChanger().mesh();
  const primitiveFacePatch& masterPatch =
    mesh.faceZones()[masterFaceZoneID_.index()]();
  const primitiveFacePatch& slavePatch =
    mesh.faceZones()[slaveFaceZoneID_.index()]();
  // Get references to local points, local edges and local faces
  // for master and slave patch
  const pointField& masterLocalPoints = masterPatch.localPoints();
  const faceList& masterLocalFaces = masterPatch.localFaces();
  const edgeList& masterEdges = masterPatch.edges();
  const labelListList& masterEdgeFaces = masterPatch.edgeFaces();
  const labelListList& masterFaceEdges = masterPatch.faceEdges();
  const labelListList& masterFaceFaces = masterPatch.faceFaces();
  const pointField& slaveLocalPoints = slavePatch.localPoints();
  const edgeList& slaveEdges = slavePatch.edges();
  const labelListList& slaveEdgeFaces = slavePatch.edgeFaces();
  const vectorField& slavePointNormals = slavePatch.pointNormals();
  // Calculate min edge distance for points and faces
  // Calculate min edge length for the points and faces of master patch
  scalarField minMasterPointLength{masterLocalPoints.size(), GREAT};
  scalarField minMasterFaceLength{masterPatch.size(), GREAT};
  FOR_ALL(masterEdges, edgeI) {
    const edge& curEdge = masterEdges[edgeI];
    const scalar curLength = masterEdges[edgeI].mag(masterLocalPoints);
    // Do points
    minMasterPointLength[curEdge.start()] =
      min(minMasterPointLength[curEdge.start()], curLength);
    minMasterPointLength[curEdge.end()] =
      min(minMasterPointLength[curEdge.end()], curLength);
    // Do faces
    const labelList& curFaces = masterEdgeFaces[edgeI];
    FOR_ALL(curFaces, faceI) {
      minMasterFaceLength[curFaces[faceI]] =
        min(minMasterFaceLength[curFaces[faceI]], curLength);
    }
  }
  // Calculate min edge length for the points and faces of slave patch
  scalarField minSlavePointLength{slaveLocalPoints.size(), GREAT};
  scalarField minSlaveFaceLength{slavePatch.size(), GREAT};
  FOR_ALL(slaveEdges, edgeI) {
    const edge& curEdge = slaveEdges[edgeI];
    const scalar curLength = slaveEdges[edgeI].mag(slaveLocalPoints);
    // Do points
    minSlavePointLength[curEdge.start()] =
      min(minSlavePointLength[curEdge.start()], curLength);
    minSlavePointLength[curEdge.end()] =
      min(minSlavePointLength[curEdge.end()], curLength);
    // Do faces
    const labelList& curFaces = slaveEdgeFaces[edgeI];
    FOR_ALL(curFaces, faceI) {
      minSlaveFaceLength[curFaces[faceI]] =
        min(minSlaveFaceLength[curFaces[faceI]], curLength);
    }
  }
  // Project slave points onto the master patch
  // Face hit by the slave point
  List<objectHit> slavePointFaceHits =
    slavePatch.projectPoints(masterPatch, slavePointNormals, projectionAlgo_);
  if (debug) {
    label nHits = 0;
    FOR_ALL(slavePointFaceHits, pointI) {
      if (slavePointFaceHits[pointI].hit()) {
        nHits++;
      }
    }
    Pout << "Number of hits in point projection: " << nHits
      << " out of " << slavePointFaceHits.size() << " points."
      << endl;
  }
  // Projected slave points are stored for mesh motion correction
  if (projectedSlavePointsPtr_)
    delete projectedSlavePointsPtr_;
  projectedSlavePointsPtr_ =
    new pointField{slavePointFaceHits.size(), vector::zero};
  pointField& projectedSlavePoints = *projectedSlavePointsPtr_;
  // Adjust projection to type of match
  label nAdjustedPoints = 0;
  // If the match is integral and some points have missed,
  // find nearest master face
  if (matchType_ == INTEGRAL) {
    if (debug) {
      Pout << "bool slidingInterface::projectPoints() for object "
        << name() << " : "
        << "Adjusting point projection for integral match: ";
    }
    FOR_ALL(slavePointFaceHits, pointI) {
      if (slavePointFaceHits[pointI].hit()) {
        // Grab the hit point
        projectedSlavePoints[pointI] =
          masterLocalFaces[slavePointFaceHits[pointI].hitObject()]
          .ray(slaveLocalPoints[pointI],
               slavePointNormals[pointI],
               masterLocalPoints,
               projectionAlgo_)
          .hitPoint();
      } else {
        // Grab the nearest point on the face (edge)
        pointHit missAdjust =
          masterLocalFaces[slavePointFaceHits[pointI].hitObject()]
          .ray(slaveLocalPoints[pointI],
               slavePointNormals[pointI],
               masterLocalPoints,
               projectionAlgo_);
        const point nearPoint = missAdjust.missPoint();
        const point missPoint = slaveLocalPoints[pointI]
          + slavePointNormals[pointI]*missAdjust.distance();
        // Calculate the tolerance
        const scalar mergeTol =
          integralAdjTol_*minSlavePointLength[pointI];
        // Adjust the hit
        if (mag(nearPoint - missPoint) < mergeTol) {
          if (debug) {
            Pout << "a";
          }
          projectedSlavePoints[pointI] = nearPoint;
          slavePointFaceHits[pointI] =
            objectHit(true, slavePointFaceHits[pointI].hitObject());
          nAdjustedPoints++;
        } else {
          projectedSlavePoints[pointI] = slaveLocalPoints[pointI];
          if (debug) {
            Pout << "n";
          }
        }
      }
    }
    if (debug) {
      Pout << " done." << endl;
    }
  } else if (matchType_ == PARTIAL) {
    FOR_ALL(slavePointFaceHits, pointI) {
      if (slavePointFaceHits[pointI].hit()) {
        // Grab the hit point
        projectedSlavePoints[pointI] =
          masterLocalFaces[slavePointFaceHits[pointI].hitObject()]
          .ray(slaveLocalPoints[pointI],
               slavePointNormals[pointI],
               masterLocalPoints,
               projectionAlgo_)
          .hitPoint();
      } else {
        // The point remains where it started from
        projectedSlavePoints[pointI] = slaveLocalPoints[pointI];
      }
    }
  } else {
    FATAL_ERROR_IN
    (
      "bool slidingInterface::projectPoints() const"
    )
    << "Problem in point projection.  Unknown sliding match type "
    << " for object " << name()
    << abort(FatalError);
  }
  if (debug) {
    Pout << "Number of adjusted points in projection: "
      << nAdjustedPoints << endl;
    // Check for zero-length edges in slave projection
    scalar minEdgeLength = GREAT;
    scalar el = 0;
    label nShortEdges = 0;
    FOR_ALL(slaveEdges, edgeI) {
      el = slaveEdges[edgeI].mag(projectedSlavePoints);
      if (el < SMALL) {
        Pout << "Point projection problems for edge: "
          << slaveEdges[edgeI] << ". Length = " << el
          << endl;
        nShortEdges++;
      }
      minEdgeLength = min(minEdgeLength, el);
    }
    if (nShortEdges > 0) {
      FATAL_ERROR_IN
      (
        "bool slidingInterface::projectPoints() const"
      )
      << "Problem in point projection.  " << nShortEdges
      << " short projected edges "
      << "after adjustment for object " << name()
      << abort(FatalError);
    } else {
      Pout << " ... projection OK." << endl;
    }
  }
  // Merge projected points against master points
  labelList slavePointPointHits{slaveLocalPoints.size(), -1};
  labelList masterPointPointHits{masterLocalPoints.size(), -1};
  // Go through all the slave points and compare them against all the points
  // in the master face they hit.  If the distance between the projected point
  // and any of the master face points is smaller than both tolerances,
  // merge the projected point by:
  // 1) adjusting the projected point to coincide with the
  //    master point it merges with
  // 2) remembering the hit master point index in slavePointPointHits
  // 3) resetting the hit face to -1
  // 4) If a master point has been hit directly, it cannot be merged
  // into the edge. Mark it as used in the reverse list
  label nMergedPoints = 0;
  FOR_ALL(projectedSlavePoints, pointI) {
    if (slavePointFaceHits[pointI].hit()) {
      // Taking a non-const reference so the point can be adjusted
      point& curPoint = projectedSlavePoints[pointI];
      // Get the hit face
      const face& hitFace =
        masterLocalFaces[slavePointFaceHits[pointI].hitObject()];
      label mergePoint = -1;
      scalar mergeDist = GREAT;
      // Try all point before deciding on best fit.
      FOR_ALL(hitFace, hitPointI) {
        scalar dist =
          mag(masterLocalPoints[hitFace[hitPointI]] - curPoint);
        // Calculate the tolerance
        const scalar mergeTol =
          pointMergeTol_*min(minSlavePointLength[pointI],
                             minMasterPointLength[hitFace[hitPointI]]);
        if (dist < mergeTol && dist < mergeDist) {
          mergePoint = hitFace[hitPointI];
          mergeDist = dist;
        }
      }
      if (mergePoint > -1) {
        // Point is to be merged with master point
        nMergedPoints++;
        slavePointPointHits[pointI] = mergePoint;
        curPoint = masterLocalPoints[mergePoint];
        masterPointPointHits[mergePoint] = pointI;
      }
    }
  }
  if (debug) {
    // Check for zero-length edges in slave projection
    scalar minEdgeLength = GREAT;
    scalar el = 0;
    FOR_ALL(slaveEdges, edgeI) {
      el = slaveEdges[edgeI].mag(projectedSlavePoints);
      if (el < SMALL) {
        Pout << "Point projection problems for edge: "
          << slaveEdges[edgeI] << ". Length = " << el
          << endl;
      }
      minEdgeLength = min(minEdgeLength, el);
    }
    if (minEdgeLength < SMALL) {
      FATAL_ERROR_IN
      (
        "bool slidingInterface::projectPoints() const"
      )
      << "Problem in point projection.  Short projected edge "
      << " after point merge for object " << name()
      << abort(FatalError);
    } else {
      Pout << " ... point merge OK." << endl;
    }
  }
  // Merge projected points against master edges
  labelList slavePointEdgeHits{slaveLocalPoints.size(), -1};
  label nMovedPoints = 0;
  FOR_ALL(projectedSlavePoints, pointI) {
    // Eliminate the points merged into points
    if (slavePointPointHits[pointI] < 0) {
      // Get current point position
      point& curPoint = projectedSlavePoints[pointI];
      // Get the hit face
      const labelList& hitFaceEdges =
        masterFaceEdges[slavePointFaceHits[pointI].hitObject()];
      // Calculate the tolerance
      const scalar mergeLength =
        min(minSlavePointLength[pointI],
            minMasterFaceLength[slavePointFaceHits[pointI].hitObject()]);
      const scalar mergeTol = pointMergeTol_*mergeLength;
      scalar minDistance = GREAT;
      FOR_ALL(hitFaceEdges, edgeI) {
        const edge& curEdge = masterEdges[hitFaceEdges[edgeI]];
        pointHit edgeHit =
          curEdge.line(masterLocalPoints).nearestDist(curPoint);
        if (edgeHit.hit()) {
          scalar dist =
            mag(edgeHit.hitPoint() - projectedSlavePoints[pointI]);
          if (dist < mergeTol && dist < minDistance) {
            // Point is to be moved onto master edge
            nMovedPoints++;
            slavePointEdgeHits[pointI] = hitFaceEdges[edgeI];
            projectedSlavePoints[pointI] = edgeHit.hitPoint();
            minDistance = dist;
          }
        }
      } // end of hit face edges
      if (slavePointEdgeHits[pointI] > -1) {
        // Projected slave point has moved.  Re-attempt merge with
        // master points of the edge
        point& curPoint = projectedSlavePoints[pointI];
        const edge& hitMasterEdge =
          masterEdges[slavePointEdgeHits[pointI]];
        label mergePoint = -1;
        scalar mergeDist = GREAT;
        FOR_ALL(hitMasterEdge, hmeI) {
          scalar hmeDist =
            mag(masterLocalPoints[hitMasterEdge[hmeI]] - curPoint);
          // Calculate the tolerance
          const scalar mergeTol =
            pointMergeTol_*min(minSlavePointLength[pointI],
                               minMasterPointLength[hitMasterEdge[hmeI]]);
          if (hmeDist < mergeTol && hmeDist < mergeDist) {
            mergePoint = hitMasterEdge[hmeI];
            mergeDist = hmeDist;
          }
        }
        if (mergePoint > -1) {
          // Point is to be merged with master point
          slavePointPointHits[pointI] = mergePoint;
          curPoint = masterLocalPoints[mergePoint];
          masterPointPointHits[mergePoint] = pointI;
          slavePointFaceHits[pointI] =
            objectHit(true, slavePointFaceHits[pointI].hitObject());
          // Disable edge merge
          slavePointEdgeHits[pointI] = -1;
        }
      }
    }
  }
  if (debug) {
    Pout << "Number of merged master points: " << nMergedPoints << nl
      << "Number of adjusted slave points: " << nMovedPoints << endl;
    // Check for zero-length edges in slave projection
    scalar minEdgeLength = GREAT;
    scalar el = 0;
    FOR_ALL(slaveEdges, edgeI) {
      el = slaveEdges[edgeI].mag(projectedSlavePoints);
      if (el < SMALL) {
        Pout << "Point projection problems for edge: "
          << slaveEdges[edgeI] << ". Length = " << el
          << endl;
      }
      minEdgeLength = min(minEdgeLength, el);
    }
    if (minEdgeLength < SMALL) {
      FATAL_ERROR_IN
      (
        "bool slidingInterface::projectPoints() const"
      )
      << "Problem in point projection.  Short projected edge "
      << " after correction for object " << name()
      << abort(FatalError);
    }
  }
  // Insert the master points into closest slave edge if appropriate
  // Algorithm:
  //    The face potentially interacts with all the points of the
  //    faces covering the path between its two ends.  Since we are
  //    examining an arbitrary shadow of the edge on a non-Euclidian
  //    surface, it is typically quite hard to do a geometric point
  //    search (under a shadow of a straight line).  Therefore, the
  //    search will be done topologically.
  //
  // I) Point collection
  // -------------------
  // 1) Grab the master faces to which the end points of the edge
  //    have been projected.
  // 2) Starting from the face containing the edge start, grab all
  //    its points and put them into the point lookup map.  Put the
  //    face onto the face lookup map.
  // 3) If the face of the end point is on the face lookup, complete
  //    the point collection step (this is checked during insertion.
  // 4) Start a new round of insertion.  Visit all faces in the face
  //    lookup and add their neighbours which are not already on the
  //    map.  Every time the new neighbour is found, add its points to
  //    the point lookup.  If the face of the end point is inserted,
  //    continue with the current roundof insertion and stop the
  //    algorithm.
  //
  // II) Point check
  // ---------------
  //    Grab all the points from the point collection and check them
  //    against the current edge.  If the edge-to-point distance is
  //    smaller than the smallest tolerance in the game (min of
  //    master point tolerance and left and right slave face around
  //    the edge tolerance) and the nearest point hits the edge, the
  //    master point will break the slave edge.  Check the actual
  //    distance of the original master position from the edge.  If
  //    the distance is smaller than a fraction of slave edge
  //    length, the hit is considered valid.  Store the slave edge
  //    index for every master point.
  labelList masterPointEdgeHits{masterLocalPoints.size(), -1};
  scalarField masterPointEdgeDist{masterLocalPoints.size(), GREAT};
  // Note.  "Processing slave edges" code is repeated twice in the
  // sliding intergace coupling in order to allow the point
  // projection to be done separately from the actual cutting.
  // Please change consistently with coupleSlidingInterface.C
  //
  if (debug) {
    Pout << "Processing slave edges " << endl;
  }
  // Create a map of faces the edge can interact with
  labelHashSet
    curFaceMap{nFacesPerSlaveEdge_*primitiveMesh::edgesPerFace_};
  labelHashSet addedFaces{2*primitiveMesh::edgesPerFace_};
  FOR_ALL(slaveEdges, edgeI) {
    const edge& curEdge = slaveEdges[edgeI];

    {
      // Clear the maps
      curFaceMap.clear();
      addedFaces.clear();
      // Grab the faces for start and end points
      const label startFace =
        slavePointFaceHits[curEdge.start()].hitObject();
      const label endFace = slavePointFaceHits[curEdge.end()].hitObject();
      // Insert the start face into the list
      curFaceMap.insert(startFace);
      addedFaces.insert(startFace);
      // If the end face is on the list, the face collection is finished
      label nSweeps = 0;
      bool completed = false;
      while (nSweeps < edgeFaceEscapeLimit_) {
        nSweeps++;
        if (addedFaces.found(endFace)) {
          completed = true;
        }
        // Add all face neighbours of face in the map
        const labelList cf = addedFaces.toc();
        addedFaces.clear();
        FOR_ALL(cf, cfI) {
          const labelList& curNbrs = masterFaceFaces[cf[cfI]];
          FOR_ALL(curNbrs, nbrI) {
            if (!curFaceMap.found(curNbrs[nbrI])) {
              curFaceMap.insert(curNbrs[nbrI]);
              addedFaces.insert(curNbrs[nbrI]);
            }
          }
        }
        if (completed)
          break;
        if (debug) {
          Pout << ".";
        }
      }
      if (!completed) {
        if (debug) {
          Pout << "x";
        }
        // It is impossible to reach the end from the start, probably
        // due to disconnected domain.  Do search in opposite direction
        label nReverseSweeps = 0;
        addedFaces.clear();
        curFaceMap.insert(endFace);
        addedFaces.insert(endFace);
        while (nReverseSweeps < edgeFaceEscapeLimit_) {
          nReverseSweeps++;
          if (addedFaces.found(startFace)) {
            completed = true;
          }
          // Add all face neighbours of face in the map
          const labelList cf = addedFaces.toc();
          addedFaces.clear();
          FOR_ALL(cf, cfI) {
            const labelList& curNbrs = masterFaceFaces[cf[cfI]];
            FOR_ALL(curNbrs, nbrI) {
              if (!curFaceMap.found(curNbrs[nbrI])) {
                curFaceMap.insert(curNbrs[nbrI]);
                addedFaces.insert(curNbrs[nbrI]);
              }
            }
          }
          if (completed)
            break;
          if (debug) {
            Pout << ".";
          }
        }
      }
      if (completed) {
        if (debug) {
          Pout << "+ ";
        }
      } else {
        if (debug) {
          Pout << "z ";
        }
      }
      // Collect the points
      // Create a map of points the edge can interact with
      labelHashSet
        curPointMap{nFacesPerSlaveEdge_*primitiveMesh::pointsPerFace_};
      const labelList curFaces = curFaceMap.toc();
      FOR_ALL(curFaces, faceI) {
        const face& f = masterLocalFaces[curFaces[faceI]];
        FOR_ALL(f, pointI) {
          curPointMap.insert(f[pointI]);
        }
      }
      const labelList curMasterPoints = curPointMap.toc();
      // Check all the points against the edge.
      linePointRef edgeLine = curEdge.line(projectedSlavePoints);
      const vector edgeVec = edgeLine.vec();
      const scalar edgeMag = edgeLine.mag();
      // Calculate actual distance involved in projection.  This
      // is used to reject master points out of reach.
      // Calculated as a combination of travel distance in projection and
      // edge length
      scalar slaveCatchDist =
        edgeMasterCatchFraction_*edgeMag
        + 0.5*(mag(projectedSlavePoints[curEdge.start()]
                   - slaveLocalPoints[curEdge.start()])
               + mag(projectedSlavePoints[curEdge.end()]
                     - slaveLocalPoints[curEdge.end()]));
      // The point merge distance needs to be measured in the
      // plane of the slave edge.  The unit vector is calculated
      // as a cross product of the edge vector and the edge
      // projection direction.  When checking for the distance
      // in plane, a minimum of the master-to-edge and
      // projected-master-to-edge distance is used, to avoid
      // problems with badly defined master planes.  HJ,
      // 17/Oct/2004
      vector edgeNormalInPlane =
        edgeVec ^ (slavePointNormals[curEdge.start()]
                   + slavePointNormals[curEdge.end()]);
      edgeNormalInPlane /= mag(edgeNormalInPlane);
      FOR_ALL(curMasterPoints, pointI) {
        const label cmp = curMasterPoints[pointI];
        // Skip the current point if the edge start or end has
        // been adjusted onto in
        if (slavePointPointHits[curEdge.start()] == cmp
            || slavePointPointHits[curEdge.end()] == cmp
            || masterPointPointHits[cmp] > -1) {
          continue;
        }
        // Check if the point actually hits the edge within bounds
        pointHit edgeLineHit =
          edgeLine.nearestDist(masterLocalPoints[cmp]);
        if (edgeLineHit.hit()) {
          // If the distance to the line is smaller than
          // the tolerance the master point needs to be
          // inserted into the edge
          // Strict checking of slave cut to avoid capturing
          // end points.
          scalar cutOnSlave =
            ((edgeLineHit.hitPoint() - edgeLine.start()) & edgeVec)/sqr(edgeMag);
          scalar distInEdgePlane =
            min(edgeLineHit.distance(),
                mag((masterLocalPoints[cmp] - edgeLineHit.hitPoint())
                    & edgeNormalInPlane));
          // Not a point hit, check for edge
          if (cutOnSlave > edgeEndCutoffTol_
              && cutOnSlave < 1.0 - edgeEndCutoffTol_ // check edge cut
              && distInEdgePlane < edgeMergeTol_*edgeMag // merge plane
              && edgeLineHit.distance()
              < min(slaveCatchDist, masterPointEdgeDist[cmp])) {
            if (debug) {
              if (masterPointEdgeHits[cmp] == -1) {
                // First hit
                Pout << "m";
              } else {
                // Repeat hit
                Pout << "M";
              }
            }
            // Snap to point onto edge
            masterPointEdgeHits[cmp] = edgeI;
            masterPointEdgeDist[cmp] = edgeLineHit.distance();
          }
        }
      }
    } // End if both ends missing
  } // End all slave edges
  if (debug) {
    Pout << endl;
  }
  if (debug) {
    Pout << "bool slidingInterface::projectPoints() for object "
      << name() << " : "
      << "Finished projecting  points. Topology = ";
  }
  // The four lists:
  // - slavePointPointHits
  // - slavePointEdgeHits
  // - slavePointFaceHits
  // - masterPointEdgeHits
  //   define how the two patches will be merged topologically.
  //   If the lists have not changed since the last merge, the
  //   sliding interface changes only geometrically and simple mesh
  //   motion will suffice.  Otherwise, a topological change is
  //   required.
  // Compare the result with the current state
  if (!attached_) {
    // The mesh needs to change topologically
    trigger_ = true;
    // Store the addressing arrays and projected points
    deleteDemandDrivenData(slavePointPointHitsPtr_);
    slavePointPointHitsPtr_ = new labelList{slavePointPointHits};
    deleteDemandDrivenData(slavePointEdgeHitsPtr_);
    slavePointEdgeHitsPtr_ = new labelList{slavePointEdgeHits};
    deleteDemandDrivenData(slavePointFaceHitsPtr_);
    slavePointFaceHitsPtr_ = new List<objectHit>{slavePointFaceHits};
    deleteDemandDrivenData(masterPointEdgeHitsPtr_);
    masterPointEdgeHitsPtr_ = new labelList{masterPointEdgeHits};
    if (debug) {
      Pout << "(Detached interface) changing." << endl;
    }
  } else {
    // Compare the lists against the stored lists.  If any of them
    // has changed, topological change will be executed.
    trigger_ = false;
    if (!slavePointPointHitsPtr_
        || !slavePointEdgeHitsPtr_
        || !slavePointFaceHitsPtr_
        || !masterPointEdgeHitsPtr_) {
      // Must be restart.  Force topological change.
      deleteDemandDrivenData(slavePointPointHitsPtr_);
      slavePointPointHitsPtr_ = new labelList{slavePointPointHits};
      deleteDemandDrivenData(slavePointEdgeHitsPtr_);
      slavePointEdgeHitsPtr_ = new labelList{slavePointEdgeHits};
      deleteDemandDrivenData(slavePointFaceHitsPtr_);
      slavePointFaceHitsPtr_ = new List<objectHit>{slavePointFaceHits};
      deleteDemandDrivenData(masterPointEdgeHitsPtr_);
      masterPointEdgeHitsPtr_ = new labelList{masterPointEdgeHits};
      if (debug) {
        Pout << "(Attached interface restart) changing." << endl;
      }
      trigger_ = true;
      return trigger_;
    }
    if (slavePointPointHits != (*slavePointPointHitsPtr_)) {
      if (debug) {
        Pout << "(Point projection) ";
      }
      trigger_ = true;
    }
    if (slavePointEdgeHits != (*slavePointEdgeHitsPtr_)) {
      if (debug) {
        Pout << "(Edge projection) ";
      }
      trigger_ = true;
    }
    // Comparison for face hits needs to exclude points that have hit
    // another point or edge
    bool faceHitsDifferent = false;
    const List<objectHit>& oldPointFaceHits = *slavePointFaceHitsPtr_;
    FOR_ALL(slavePointFaceHits, pointI) {
      if (slavePointPointHits[pointI] < 0 && slavePointEdgeHits[pointI] < 0) {
        // This is a straight face hit
        if (slavePointFaceHits[pointI] != oldPointFaceHits[pointI]) {
          // Two lists are different
          faceHitsDifferent = true;
          break;
        }
      }
    }
    if (faceHitsDifferent) {
      if (debug) {
        Pout << "(Face projection) ";
      }
      trigger_ = true;
    }
    if (masterPointEdgeHits != (*masterPointEdgeHitsPtr_)) {
      if (debug) {
        Pout << "(Master point projection) ";
      }
      trigger_ = true;
    }
    if (trigger_) {
      // Grab new data
      deleteDemandDrivenData(slavePointPointHitsPtr_);
      slavePointPointHitsPtr_ = new labelList{slavePointPointHits};
      deleteDemandDrivenData(slavePointEdgeHitsPtr_);
      slavePointEdgeHitsPtr_ = new labelList{slavePointEdgeHits};
      deleteDemandDrivenData(slavePointFaceHitsPtr_);
      slavePointFaceHitsPtr_ = new List<objectHit>{slavePointFaceHits};
      deleteDemandDrivenData(masterPointEdgeHitsPtr_);
      masterPointEdgeHitsPtr_ = new labelList{masterPointEdgeHits};
      if (debug) {
        Pout << "changing." << endl;
      }
    } else {
      if (debug) {
        Pout << "preserved." << endl;
      }
    }
  }
  return trigger_;
}


void mousse::slidingInterface::clearPointProjection() const
{
  deleteDemandDrivenData(slavePointPointHitsPtr_);
  deleteDemandDrivenData(slavePointEdgeHitsPtr_);
  deleteDemandDrivenData(slavePointFaceHitsPtr_);
  deleteDemandDrivenData(masterPointEdgeHitsPtr_);
  deleteDemandDrivenData(projectedSlavePointsPtr_);
}

