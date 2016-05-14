// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "body_centred_cubic.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(bodyCentredCubic, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(initialPointsMethod, bodyCentredCubic, dictionary);


// Constructors 
bodyCentredCubic::bodyCentredCubic
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
  initialCellSize_{readScalar(detailsDict().lookup("initialCellSize"))},
  randomiseInitialGrid_{detailsDict().lookup("randomiseInitialGrid")},
  randomPerturbationCoeff_
  {
    readScalar(detailsDict().lookup("randomPerturbationCoeff"))
  }
{}


// Member Functions 
List<Vb::Point> bodyCentredCubic::initialPoints() const
{
  boundBox bb;
  // Pick up the bounds of this processor, or the whole geometry, depending
  // on whether this is a parallel run.
  if (Pstream::parRun()) {
    bb = decomposition().procBounds();
  } else {
    bb = geometryToConformTo().globalBounds();
  }
  scalar x0 = bb.min().x();
  scalar xR = bb.max().x() - x0;
  label ni = label(xR/initialCellSize_);
  scalar y0 = bb.min().y();
  scalar yR = bb.max().y() - y0;
  label nj = label(yR/initialCellSize_);
  scalar z0 = bb.min().z();
  scalar zR = bb.max().z() - z0;
  label nk = label(zR/initialCellSize_);
  vector delta(xR/ni, yR/nj, zR/nk);
  delta *= pow((1.0/2.0),-(1.0/3.0));
  scalar pert = randomPerturbationCoeff_*cmptMin(delta);
  DynamicList<Vb::Point> initialPoints{ni*nj*nk/10};
  for (label i = 0; i < ni; i++) {
    for (label j = 0; j < nj; j++) {
      // Generating, testing and adding points one line at a time to
      // reduce the memory requirement for cases with bounding boxes that
      // are very large in comparison to the volume to be filled
      label pI = 0;
      pointField points{2*nk};
      for (label k = 0; k < nk; k++) {
        point pA
        {
          x0 + i*delta.x(),
          y0 + j*delta.y(),
          z0 + k*delta.z()
        };
        point pB = pA + 0.5*delta;
        if (randomiseInitialGrid_) {
          pA.x() += pert*(rndGen().scalar01() - 0.5);
          pA.y() += pert*(rndGen().scalar01() - 0.5);
          pA.z() += pert*(rndGen().scalar01() - 0.5);
        }
        if (Pstream::parRun()) {
          if (decomposition().positionOnThisProcessor(pA)) {
            // Add this point in parallel only if this position is
            // on this processor.
            points[pI++] = pA;
          }
        } else {
          points[pI++] = pA;
        }
        if (randomiseInitialGrid_) {
          pB.x() += pert*(rndGen().scalar01() - 0.5);
          pB.y() += pert*(rndGen().scalar01() - 0.5);
          pB.z() += pert*(rndGen().scalar01() - 0.5);
        }
        if (Pstream::parRun()) {
          if (decomposition().positionOnThisProcessor(pB)) {
            // Add this point in parallel only if this position is
            // on this processor.
            points[pI++] = pB;
          }
        } else {
          points[pI++] = pB;
        }
      }
      points.setSize(pI);
      Field<bool> insidePoints =
        geometryToConformTo().wellInside
        (
          points,
          minimumSurfaceDistanceCoeffSqr_
          *sqr(cellShapeControls().cellSize(points))
        );
      FOR_ALL(insidePoints, i) {
        if (insidePoints[i]) {
          const point& p(points[i]);
          initialPoints.append(Vb::Point(p.x(), p.y(), p.z()));
        }
      }
    }
  }
  return initialPoints.shrink();
}

}  // namespace mousse

