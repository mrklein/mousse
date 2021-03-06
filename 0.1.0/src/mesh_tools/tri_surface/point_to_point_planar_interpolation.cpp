// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_to_point_planar_interpolation.hpp"
#include "bound_box.hpp"
#include "random.hpp"
#include "vector_2d.hpp"
#include "tri_surface.hpp"
#include "tri_surface_tools.hpp"
#include "obj_stream.hpp"
#include "time.hpp"
#include "match_points.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(pointToPointPlanarInterpolation, 0);

}


// Private Member Functions 
mousse::coordinateSystem
mousse::pointToPointPlanarInterpolation::calcCoordinateSystem
(
  const pointField& points
) const
{
  if (points.size() < 3) {
    FATAL_ERROR_IN
    (
      "pointToPointPlanarInterpolation::calcCoordinateSystem"
      "(const pointField&)"
    )
    << "Only " << points.size() << " provided." << nl
    << "Need at least three non-colinear points"
    << " to be able to interpolate."
    << exit(FatalError);
  }
  const point& p0 = points[0];
  // Find furthest away point
  vector e1;
  label index1 = -1;
  scalar maxDist = -GREAT;
  for (label i = 1; i < points.size(); i++) {
    const vector d = points[i] - p0;
    scalar magD = mag(d);
    if (magD > maxDist) {
      e1 = d/magD;
      index1 = i;
      maxDist = magD;
    }
  }
  // Find point that is furthest away from line p0-p1
  const point& p1 = points[index1];
  label index2 = -1;
  maxDist = -GREAT;
  for (label i = 1; i < points.size(); i++) {
    if (i == index1)
      continue;
    const point& p2 = points[i];
    vector e2(p2 - p0);
    e2 -= (e2&e1)*e1;
    scalar magE2 = mag(e2);
    if (magE2 > maxDist) {
      index2 = i;
      maxDist = magE2;
    }
  }
  if (index2 == -1) {
    FATAL_ERROR_IN
    (
      "pointToPointPlanarInterpolation::calcCoordinateSystem"
      "(const pointField&)"
    )
    << "Cannot find points that make valid normal." << nl
    << "Have so far points " << p0 << " and " << p1
    << "Need at least three points which are not in a line."
    << exit(FatalError);
  }
  vector n = e1^(points[index2]-p0);
  n /= mag(n);
  if (debug) {
    Info << "pointToPointPlanarInterpolation::calcCoordinateSystem :"
      << " Used points " << p0 << ' ' << points[index1]
      << ' ' << points[index2]
      << " to define coordinate system with normal " << n << endl;
  }
  return // coordinateSystem
  {
    "reference",
    p0,  // origin
    n,   // normal
    e1   // 0-axis
  };
}


void mousse::pointToPointPlanarInterpolation::calcWeights
(
  const pointField& sourcePoints,
  const pointField& destPoints
)
{
  if (nearestOnly_) {
    labelList destToSource;
    bool fullMatch = matchPoints
    (
      destPoints,
      sourcePoints,
      scalarField(destPoints.size(), GREAT),
      true,       // verbose
      destToSource
    );
    if (!fullMatch) {
      FATAL_ERROR_IN("pointToPointPlanarInterpolation::calcWeights(..)")
        << "Did not find a corresponding sourcePoint for every face"
        << " centre" << exit(FatalError);
    }
    nearestVertex_.setSize(destPoints.size());
    nearestVertexWeight_.setSize(destPoints.size());
    FOR_ALL(nearestVertex_, i) {
      nearestVertex_[i][0] = destToSource[i];
      nearestVertex_[i][1] = -1;
      nearestVertex_[i][2] = -1;
      nearestVertexWeight_[i][0] = 1.0;
      nearestVertexWeight_[i][1] = 0.0;
      nearestVertexWeight_[i][2] = 0.0;
    }
    if (debug) {
      FOR_ALL(destPoints, i) {
        label v0 = nearestVertex_[i][0];
        Pout << "For location " << destPoints[i]
          << " sampling vertex " << v0
          << " at:" << sourcePoints[v0]
          << " distance:" << mag(sourcePoints[v0]-destPoints[i])
          << endl;
      }
      OBJstream str{"destToSource.obj"};
      Pout << "pointToPointPlanarInterpolation::calcWeights :"
        << " Dumping lines from face centres to original points to "
        << str.name() << endl;
      FOR_ALL(destPoints, i) {
        label v0 = nearestVertex_[i][0];
        str.write(linePointRef(destPoints[i], sourcePoints[v0]));
      }
    }
  } else {
    tmp<vectorField> tlocalVertices
    {
      referenceCS_.localPosition(sourcePoints)
    };
    vectorField& localVertices = tlocalVertices();
    const boundBox bb(localVertices, true);
    const point bbMid(bb.midpoint());
    if (debug) {
      Info << "pointToPointPlanarInterpolation::calcWeights :"
        << " Perturbing points with " << perturb_
        << " fraction of a random position inside " << bb
        << " to break any ties on regular meshes."
        << nl << endl;
    }
    Random rndGen{123456};
    FOR_ALL(localVertices, i) {
      localVertices[i] +=
        perturb_*(rndGen.position(bb.min(), bb.max())-bbMid);
    }
    // Determine triangulation
    List<vector2D> localVertices2D{localVertices.size()};
    FOR_ALL(localVertices, i) {
      localVertices2D[i][0] = localVertices[i][0];
      localVertices2D[i][1] = localVertices[i][1];
    }
    triSurface s{triSurfaceTools::delaunay2D(localVertices2D)};
    tmp<pointField> tlocalFaceCentres
    {
      referenceCS_.localPosition(destPoints)
    };
    const pointField& localFaceCentres = tlocalFaceCentres();
    if (debug) {
      Pout << "pointToPointPlanarInterpolation::calcWeights :"
        <<" Dumping triangulated surface to triangulation.stl" << endl;
      s.write("triangulation.stl");
      OBJstream str{"localFaceCentres.obj"};
      Pout << "pointToPointPlanarInterpolation::calcWeights :"
        << " Dumping face centres to " << str.name() << endl;
      FOR_ALL(localFaceCentres, i) {
        str.write(localFaceCentres[i]);
      }
    }
    // Determine interpolation onto face centres.
    triSurfaceTools::calcInterpolationWeights
    (
      s,
      localFaceCentres,   // points to interpolate to
      nearestVertex_,
      nearestVertexWeight_
    );
    if (debug) {
      FOR_ALL(sourcePoints, i) {
        Pout << "source:" << i << " at:" << sourcePoints[i]
          << " 2d:" << localVertices[i]
          << endl;
      }
      FOR_ALL(destPoints, i) {
        label v0 = nearestVertex_[i][0];
        label v1 = nearestVertex_[i][1];
        label v2 = nearestVertex_[i][2];
        Pout << "For location " << destPoints[i]
          << " 2d:" << localFaceCentres[i]
          << " sampling vertices" << nl
          << "    " << v0
          << " at:" << sourcePoints[v0]
          << " weight:" << nearestVertexWeight_[i][0] << nl;
        if (v1 != -1) {
          Pout << "    " << v1
            << " at:" << sourcePoints[v1]
            << " weight:" << nearestVertexWeight_[i][1] << nl;
        }
        if (v2 != -1) {
          Pout << "    " << v2
            << " at:" << sourcePoints[v2]
            << " weight:" << nearestVertexWeight_[i][2] << nl;
        }
        Pout << endl;
      }
    }
  }
}


// Constructors 
mousse::pointToPointPlanarInterpolation::pointToPointPlanarInterpolation
(
  const pointField& sourcePoints,
  const pointField& destPoints,
  const scalar perturb,
  const bool nearestOnly
)
:
  perturb_{perturb},
  nearestOnly_{nearestOnly},
  referenceCS_{calcCoordinateSystem(sourcePoints)},
  nPoints_{sourcePoints.size()}
{
  calcWeights(sourcePoints, destPoints);
}


mousse::pointToPointPlanarInterpolation::pointToPointPlanarInterpolation
(
  const coordinateSystem& referenceCS,
  const pointField& sourcePoints,
  const pointField& destPoints,
  const scalar perturb
)
:
  perturb_{perturb},
  nearestOnly_{false},
  referenceCS_{referenceCS},
  nPoints_{sourcePoints.size()}
{
  calcWeights(sourcePoints, destPoints);
}


// Member Functions 
mousse::wordList mousse::pointToPointPlanarInterpolation::timeNames
(
  const instantList& times
)
{
  wordList names{times.size()};
  FOR_ALL(times, i) {
    names[i] = times[i].name();
  }
  return names;
}


bool mousse::pointToPointPlanarInterpolation::findTime
(
  const instantList& times,
  const label startSampleTime,
  const scalar timeVal,
  label& lo,
  label& hi
)
{
  lo = startSampleTime;
  hi = -1;
  for (label i = startSampleTime+1; i < times.size(); i++) {
    if (times[i].value() > timeVal) {
      break;
    } else {
      lo = i;
    }
  }
  if (lo == -1) {
    return false;
  }
  if (lo < times.size()-1) {
    hi = lo+1;
  }
  if (debug) {
    if (hi == -1) {
      Pout << "findTime : Found time " << timeVal << " after"
        << " index:" << lo << " time:" << times[lo].value()
        << endl;
    } else {
      Pout << "findTime : Found time " << timeVal << " inbetween"
        << " index:" << lo << " time:" << times[lo].value()
        << " and index:" << hi << " time:" << times[hi].value()
        << endl;
    }
  }
  return true;
}

