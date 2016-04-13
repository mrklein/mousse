// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "empty_poly_patch.hpp"
#include "twod_point_corrector.hpp"

using namespace mousse;

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_poly_mesh.inc"
  pointIOField points
  {
    {
      "points",
      runTime.findInstance(polyMesh::meshSubDir, "points"),
      polyMesh::meshSubDir,
      runTime,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    }
  };
  boundBox bb{points};
  Info << "bounding box: min = " << bb.min()
    << " max = " << bb.max() << " metres."
    << endl;
  point midPoint = gAverage(points);
  twoDPointCorrector twoDCorr{mesh};
  direction planeNormalCmpt = twoDCorr.normalDir();
  scalar midCmptVal = midPoint[planeNormalCmpt];
  scalar minCmptVal = bb.min()[planeNormalCmpt];
  scalar maxCmptVal = bb.max()[planeNormalCmpt];
  FOR_ALL(points, pointI)
  {
    if (points[pointI][planeNormalCmpt] < midCmptVal)
    {
      points[pointI][planeNormalCmpt] = minCmptVal;
    }
    else
    {
      points[pointI][planeNormalCmpt] = maxCmptVal;
    }
  }
  twoDCorr.correctPoints(points);
  // Set the precision of the points data to 10
  IOstream::defaultPrecision(max(10u, IOstream::defaultPrecision()));
  Info << "Writing points into directory " << points.path() << nl << endl;
  points.write();
  return 0;
}
