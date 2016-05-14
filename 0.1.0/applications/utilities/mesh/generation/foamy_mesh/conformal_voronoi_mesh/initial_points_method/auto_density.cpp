// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "auto_density.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(autoDensity, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  initialPointsMethod,
  autoDensity,
  dictionary
);


// Private Member Functions 
void mousse::autoDensity::writeOBJ
(
  const treeBoundBox& bb,
  fileName name
) const
{
  OFstream str{time().path()/name + ".obj"};
  Pout << "Writing " << str.name() << endl;
  pointField bbPoints{bb.points()};
  FOR_ALL(bbPoints, i) {
    meshTools::writeOBJ(str, bbPoints[i]);
  }
  FOR_ALL(treeBoundBox::edges, i) {
    const edge& e = treeBoundBox::edges[i];
    str << "l " << e[0] + 1 << ' ' << e[1] + 1 << nl;
  }
}


bool mousse::autoDensity::combinedOverlaps(const treeBoundBox& box) const
{
  if (Pstream::parRun()) {
    return
      decomposition().overlapsThisProcessor(box)
      || geometryToConformTo().overlaps(box);
  }
  return geometryToConformTo().overlaps(box);
}


bool mousse::autoDensity::combinedInside(const point& p) const
{
  if (Pstream::parRun()) {
    return
      decomposition().positionOnThisProcessor(p)
      && geometryToConformTo().inside(p);
  }
  return geometryToConformTo().inside(p);
}


mousse::Field<bool> mousse::autoDensity::combinedWellInside
(
  const pointField& pts,
  const scalarField& sizes
) const
{
  if (!Pstream::parRun()) {
    return
      geometryToConformTo().wellInside
      (
        pts,
        minimumSurfaceDistanceCoeffSqr_*sqr(sizes)
      );
  }
  Field<bool> inside{pts.size(), true};
  // Perform AND operation between testing the surfaces and the previous
  // field, i.e the parallel result, or in serial, with true.
  Field<bool> insideA
  {
    geometryToConformTo().wellInside
    (
      pts,
      minimumSurfaceDistanceCoeffSqr_*sqr(sizes)
    )
  };
  Field<bool> insideB
  {
    decomposition().positionOnThisProcessor(pts)
  };
  FOR_ALL(inside, i) {
    inside[i] = (insideA[i] && insideB[i]);
  }
  return inside;
}


bool mousse::autoDensity::combinedWellInside
(
  const point& p,
  scalar size
) const
{
  bool inside = true;
  if (Pstream::parRun()) {
    inside = decomposition().positionOnThisProcessor(p);
  }
  // Perform AND operation between testing the surfaces and the previous
  // result, i.e the parallel result, or in serial, with true.
  inside =
    inside
    && geometryToConformTo().wellInside
    (
      p,
      minimumSurfaceDistanceCoeffSqr_*sqr(size)
    );
  return inside;
}


mousse::label mousse::autoDensity::recurseAndFill
(
  DynamicList<Vb::Point>& initialPoints,
  const treeBoundBox& bb,
  label levelLimit,
  word recursionName
) const
{
  label treeDepth = 0;
  for (direction i = 0; i < 8; i++) {
    treeBoundBox subBB = bb.subBbox(i);
    word newName = recursionName + "_" + mousse::name(i);
    conformalVoronoiMesh::timeCheck(time(), newName, debug);
    if (combinedOverlaps(subBB)) {
      if (levelLimit > 0) {
        treeDepth =
          max
          (
            treeDepth,
            recurseAndFill
            (
              initialPoints,
              subBB,
              levelLimit - 1,
              newName
            )
          );
      } else {
        if (debug) {
          writeOBJ
          (
            subBB,
            word(newName + "_overlap")
          );
          Pout << newName + "_overlap " << subBB << endl;
        }
        if (!fillBox(initialPoints, subBB, true)) {
          treeDepth =
            max
            (
              treeDepth,
              recurseAndFill
              (
                initialPoints,
                subBB,
                levelLimit - 1,
                newName
              )
            );
        }
      }
    } else if (combinedInside(subBB.midpoint())) {
      if (debug) {
        writeOBJ
        (
          subBB,
          newName + "_inside"
        );
        Pout << newName + "_inside " << subBB << endl;
      }
      if (!fillBox(initialPoints, subBB, false)) {
        treeDepth =
          max
          (
            treeDepth,
            recurseAndFill
            (
              initialPoints,
              subBB,
              levelLimit - 1,
              newName
            )
          );
      }
    } else {
      if (debug) {
        writeOBJ
        (
          subBB,
          newName + "_outside"
        );
      }
    }
  }
  return treeDepth + 1;
}


bool mousse::autoDensity::fillBox
(
  DynamicList<Vb::Point>& initialPoints,
  const treeBoundBox& bb,
  bool overlapping
) const
{
  const conformationSurfaces& geometry = geometryToConformTo();
  label initialSize = initialPoints.size();
  scalar maxCellSize = -GREAT;
  scalar minCellSize = GREAT;
  scalar maxDensity = 1/pow3(minCellSize);
  scalar volumeAdded = 0.0;
  const point& min = bb.min();
  vector span = bb.span();
  scalar totalVolume = bb.volume();
  label trialPoints = 0;
  bool wellInside = false;
  if (!overlapping) {
    // Check the nearest point on the surface to the box, if it is far
    // enough away, then the surface sampling of the box can be skipped.
    // Checking if the nearest piece of surface is at least 1.5*bb.span away
    // from the bb.midpoint.
    pointIndexHit surfHit;
    label hitSurface;
    geometry.findSurfaceNearest
    (
      bb.midpoint(),
      2.25*magSqr(span),
      surfHit,
      hitSurface
    );
    if (!surfHit.hit()) {
      if (debug) {
        Pout << "box wellInside, no need to sample surface." << endl;
      }
      wellInside = true;
    }
  }
  if (!overlapping && !wellInside) {
    // If this is an inside box then it is possible to fill points very
    // close to the boundary, to prevent this, check the corners and sides
    // of the box so ensure that they are "wellInside".  If not, set as an
    // overlapping box.
    pointField corners{bb.points()};
    scalarField cornerSizes = cellShapeControls().cellSize(corners);
    Field<bool> insideCorners = combinedWellInside(corners, cornerSizes);
    FOR_ALL(insideCorners, i) {
      // Use the sizes to improve the min/max cell size estimate
      scalar s = cornerSizes[i];
      if (s > maxCellSize) {
        maxCellSize = s;
      }
      if (s < minCellSize) {
        minCellSize = max(s, minCellSizeLimit_);
      }
      if (maxCellSize/minCellSize > maxSizeRatio_) {
        if (debug) {
          Pout << "Abort fill at corner sample stage,"
            << " minCellSize " << minCellSize
            << " maxCellSize " << maxCellSize
            << " maxSizeRatio " << maxCellSize/minCellSize
            << endl;
        }
        return false;
      }
      if (!insideCorners[i]) {
        // If one or more corners is not "wellInside", then treat this
        // as an overlapping box.
        if (debug) {
          Pout << "Inside box found to have some non-wellInside "
            << "corners, using overlapping fill."
            << endl;
        }
        overlapping = true;
        break;
      }
    }
    if (!overlapping) {
      vector delta = span/(surfRes_ - 1);
      label nLine = 6*(surfRes_ - 2);
      pointField linePoints{nLine, vector::zero};
      scalarField lineSizes{nLine, 0.0};
      for (label i = 0; i < surfRes_; i++) {
        label lPI = 0;
        for (label j = 1; j < surfRes_ - 1 ; j++) {
          linePoints[lPI++] =
            min + vector(0, delta.y()*i, delta.z()*j);
          linePoints[lPI++] =
            min + vector(delta.x()*(surfRes_ - 1),
                         delta.y()*i,
                         delta.z()*j);
          linePoints[lPI++] =
            min + vector(delta.x()*j, 0, delta.z()*i);
          linePoints[lPI++] =
            min + vector(delta.x()*j,
                         delta.y()*(surfRes_ - 1),
                         delta.z()*i);
          linePoints[lPI++] =
            min + vector(delta.x()*i, delta.y()*j, 0);
          linePoints[lPI++] =
            min + vector(delta.x()*i,
                         delta.y()*j,
                         delta.z()*(surfRes_ - 1));
        }
        lineSizes = cellShapeControls().cellSize(linePoints);
        Field<bool> insideLines =
          combinedWellInside
          (
            linePoints,
            lineSizes
          );
        FOR_ALL(insideLines, i) {
          // Use the sizes to improve the min/max cell size estimate
          scalar s = lineSizes[i];
          if (s > maxCellSize) {
            maxCellSize = s;
          }
          if (s < minCellSize) {
            minCellSize = max(s, minCellSizeLimit_);
          }
          if (maxCellSize/minCellSize > maxSizeRatio_) {
            if (debug) {
              Pout << "Abort fill at surface sample stage, "
                << " minCellSize " << minCellSize
                << " maxCellSize " << maxCellSize
                << " maxSizeRatio " << maxCellSize/minCellSize
                << endl;
            }
            return false;
          }
          if (!insideLines[i]) {
            // If one or more surface points is not "wellInside",
            // then treat this as an overlapping box.
            overlapping = true;
            if (debug) {
              Pout << "Inside box found to have some non-"
                << "wellInside surface points, using "
                << "overlapping fill."
                << endl;
            }
            break;
          }
        }
      }
    }
  }
  if (overlapping) {
    // Sample the box to find an estimate of the min size, and a volume
    // estimate when overlapping == true.
    pointField samplePoints
    {
      volRes_*volRes_*volRes_,
      vector::zero
    };
    vector delta = span/volRes_;
    label pI = 0;
    for (label i = 0; i < volRes_; i++) {
      for (label j = 0; j < volRes_; j++) {
        for (label k = 0; k < volRes_; k++) {
          // Perturb the points to avoid creating degenerate positions
          // in the Delaunay tessellation.
          samplePoints[pI++] =
            min + vector(delta.x()*(i + 0.5 + 0.1*(rndGen().scalar01() - 0.5)),
                         delta.y()*(j + 0.5 + 0.1*(rndGen().scalar01() - 0.5)),
                         delta.z()*(k + 0.5 + 0.1*(rndGen().scalar01() - 0.5)));
        }
      }
    }
    // Randomise the order of the points to (potentially) improve the speed
    // of assessing the density ratio, and prevent a box being filled from a
    // corner when only some these points are required.
    shuffle(samplePoints);
    scalarField sampleSizes = cellShapeControls().cellSize(samplePoints);
    Field<bool> insidePoints =
      combinedWellInside
      (
        samplePoints,
        sampleSizes
      );
    label nInside = 0;
    FOR_ALL(insidePoints, i) {
      if (!insidePoints[i])
        continue;
      nInside++;
      scalar s = sampleSizes[i];
      if (s > maxCellSize) {
        maxCellSize = s;
      }
      if (s < minCellSize) {
        minCellSize = max(s, minCellSizeLimit_);
      }
      if (maxCellSize/minCellSize > maxSizeRatio_) {
        if (debug) {
          Pout << "Abort fill at sample stage,"
            << " minCellSize " << minCellSize
            << " maxCellSize " << maxCellSize
            << " maxSizeRatio " << maxCellSize/minCellSize
            << endl;
        }
        return false;
      }
    }
    if (nInside == 0) {
      if (debug) {
        Pout << "No sample points found inside box" << endl;
      }
      return true;
    }
    if (debug) {
      Pout << scalar(nInside)/scalar(samplePoints.size())
        << " full overlapping box" << endl;
    }
    totalVolume *= scalar(nInside)/scalar(samplePoints.size());
    if (debug) {
      Pout << "Total volume to fill = " << totalVolume << endl;
    }
    // Using the sampledPoints as the first test locations as they are
    // randomly shuffled, but unfiormly sampling space and have wellInside
    // and size data already
    maxDensity = 1/pow3(max(minCellSize, SMALL));
    FOR_ALL(insidePoints, i) {
      if (!insidePoints[i])
        continue;
      trialPoints++;
      const point& p = samplePoints[i];
      scalar localSize = sampleSizes[i];
      scalar localDensity = 1/pow3(localSize);
      // No need to look at max/min cell size here, already handled
      // by sampling
      // Accept possible placements proportional to the relative
      // local density
      // TODO - is there a lot of cost in the 1/density calc?  Could
      // assess on
      //    (1/maxDensity)/(1/localDensity) = minVolume/localVolume
      if (localDensity/maxDensity > rndGen().scalar01()) {
        scalar localVolume = 1/localDensity;
        if (volumeAdded + localVolume > totalVolume) {
          // Add the final box with a probability of to the ratio
          // of the remaining volume to the volume to be added,
          // i.e. insert a box of volume 0.5 into a remaining
          // volume of 0.1 20% of the time.
          scalar addProbability =
           (totalVolume - volumeAdded)/localVolume;
          scalar r = rndGen().scalar01();
          if (debug) {
            Pout
              << "totalVolume " << totalVolume << nl
              << "volumeAdded " << volumeAdded << nl
              << "localVolume " << localVolume << nl
              << "addProbability " << addProbability << nl
              << "random " << r
              << endl;
          }
          if (addProbability > r) {
            // Place this volume before finishing filling this box
            initialPoints.append
            (
              Vb::Point(p.x(), p.y(), p.z())
            );
            volumeAdded += localVolume;
          }
          break;
        }
        initialPoints.append(Vb::Point(p.x(), p.y(), p.z()));
        volumeAdded += localVolume;
      }
    }
  }
  if (volumeAdded < totalVolume) {
    if (debug) {
      Pout << "Adding random points, remaining volume "
        << totalVolume - volumeAdded
        << endl;
    }
    maxDensity = 1/pow3(max(minCellSize, SMALL));
    while (true) {
      trialPoints++;
      point p = min + cmptMultiply(span, rndGen().vector01());
      scalar localSize = cellShapeControls().cellSize(p);
      bool insidePoint = false;
      if (!overlapping) {
        insidePoint = true;
      } else {
        // Determine if the point is "wellInside" the domain
        insidePoint = combinedWellInside(p, localSize);
      }
      if (insidePoint) {
        if (localSize > maxCellSize) {
          maxCellSize = localSize;
        }
        if (localSize < minCellSize) {
          minCellSize = max(localSize, minCellSizeLimit_);
          localSize = minCellSize;
          // 1/(minimum cell size)^3, gives the maximum permissible
          // point density
          maxDensity = 1/pow3(max(minCellSize, SMALL));
        }
        if (maxCellSize/minCellSize > maxSizeRatio_) {
          if (debug) {
            Pout << "Abort fill at random fill stage,"
              << " minCellSize " << minCellSize
              << " maxCellSize " << maxCellSize
              << " maxSizeRatio " << maxCellSize/minCellSize
              << endl;
          }
          // Discard any points already filled into this box by
          // setting size of initialPoints back to its starting value
          initialPoints.resize(initialSize);
          return false;
        }
        scalar localDensity = 1/pow3(max(localSize, SMALL));
        // Accept possible placements proportional to the relative local
        // density
        if (localDensity/maxDensity > rndGen().scalar01()) {
          scalar localVolume = 1/localDensity;
          if (volumeAdded + localVolume > totalVolume) {
            // Add the final box with a probability of to the ratio
            // of the remaining volume to the volume to be added,
            // i.e. insert a box of volume 0.5 into a remaining
            // volume of 0.1 20% of the time.
            scalar addProbability = (totalVolume - volumeAdded)/localVolume;
            scalar r = rndGen().scalar01();
            if (debug) {
              Pout << "totalVolume " << totalVolume << nl
                << "volumeAdded " << volumeAdded << nl
                << "localVolume " << localVolume << nl
                << "addProbability " << addProbability << nl
                << "random " << r
                << endl;
            }
            if (addProbability > r) {
              // Place this volume before finishing filling this
              // box
              // Pout<< "Final volume probability break accept"
              //     << endl;
              initialPoints.append
              (
                Vb::Point(p.x(), p.y(), p.z())
              );
              volumeAdded += localVolume;
            }
            break;
          }
          initialPoints.append(Vb::Point(p.x(), p.y(), p.z()));
          volumeAdded += localVolume;
        }
      }
    }
  }
  globalTrialPoints_ += trialPoints;
  if (debug) {
    Pout << trialPoints
      << " locations queried, " << initialPoints.size() - initialSize
      << " points placed, ("
      << scalar(initialPoints.size() - initialSize)/scalar(max(trialPoints, 1))
      << " success rate)." << nl
      << "minCellSize " << minCellSize
      << ", maxCellSize " << maxCellSize
      << ", ratio " << maxCellSize/minCellSize
      << nl << endl;
  }
  return true;
}


// Constructors 
autoDensity::autoDensity
(
  const dictionary& initialPointsDict,
  const Time& runTime,
  Random& rndGen,
  const conformationSurfaces& geometryToConformTo,
  const cellShapeControl& cellShapeControls,
  const autoPtr<backgroundMeshDecomposition>& decomposition
)
:
  initialPointsMethod
  {
    typeName,
    initialPointsDict,
    runTime,
    rndGen,
    geometryToConformTo,
    cellShapeControls,
    decomposition
  },
  globalTrialPoints_{0},
  minCellSizeLimit_
  {
    detailsDict().lookupOrDefault<scalar>("minCellSizeLimit", 0.0)
  },
  minLevels_{readLabel(detailsDict().lookup("minLevels"))},
  maxSizeRatio_{readScalar(detailsDict().lookup("maxSizeRatio"))},
  volRes_{readLabel(detailsDict().lookup("sampleResolution"))},
  surfRes_
  {
    detailsDict().lookupOrDefault<label>("surfaceSampleResolution", volRes_)
  }
{
  if (maxSizeRatio_ <= 1.0) {
    maxSizeRatio_ = 2.0;
    WARNING_IN
    (
      "autoDensity::autoDensity"
      "("
      "  const dictionary& initialPointsDict,"
      "  const conformalVoronoiMesh& foamyHexMesh"
      ")"
    )
    << "The maxSizeRatio must be greater than one to be sensible, "
    << "setting to " << maxSizeRatio_
    << endl;
  }
}


// Member Functions 
List<Vb::Point> autoDensity::initialPoints() const
{
  treeBoundBox hierBB;
  // Pick up the bounds of this processor, or the whole geometry, depending
  // on whether this is a parallel run.
  if (Pstream::parRun()) {
    hierBB = decomposition().procBounds();
  } else {
    // Extend the global box to move it off large plane surfaces
    hierBB =
      geometryToConformTo().globalBounds().extend
      (
        rndGen(),
        1e-6
      );
  }
  DynamicList<Vb::Point> initialPoints;
  Info << nl << "    " << typeName << endl;
  if (debug) {
    Pout << "    Filling box " << hierBB << endl;
  }
  label treeDepth =
    recurseAndFill
    (
      initialPoints,
      hierBB,
      minLevels_ - 1,
      "recursionBox"
    );
  initialPoints.shrink();
  label nInitialPoints = initialPoints.size();
  if (Pstream::parRun()) {
    reduce(nInitialPoints, sumOp<label>());
    reduce(globalTrialPoints_, sumOp<label>());
  }
  Info << incrIndent << incrIndent
    << indent << nInitialPoints << " points placed" << nl
    << indent << globalTrialPoints_ << " locations queried" << nl
    << indent
    << scalar(nInitialPoints)/scalar(max(globalTrialPoints_, 1))
    << " success rate" << nl
    << indent
    << returnReduce(treeDepth, maxOp<label>())
    << " levels of recursion (maximum)"
    << decrIndent << decrIndent
    << endl;
  return initialPoints;
}

}  // namespace mousse

