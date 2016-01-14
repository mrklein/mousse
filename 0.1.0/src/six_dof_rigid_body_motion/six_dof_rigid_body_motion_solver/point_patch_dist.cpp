// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_patch_dist.hpp"
#include "external_point_edge_point.hpp"
#include "point_mesh.hpp"
#include "point_edge_wave.hpp"
// Constructors 
mousse::pointPatchDist::pointPatchDist
(
  const pointMesh& pMesh,
  const labelHashSet& patchIDs,
  const pointField& points
)
:
  pointScalarField
  (
    IOobject
    (
      "pointDistance",
      pMesh.db().time().timeName(),
      pMesh.db()
    ),
    pMesh,
    dimensionedScalar("y", dimLength, GREAT)
  ),
  points_(points),
  patchIDs_(patchIDs),
  nUnset_(0)
{
  correct();
}
// Destructor 
mousse::pointPatchDist::~pointPatchDist()
{}
// Member Functions 
void mousse::pointPatchDist::correct()
{
  const pointBoundaryMesh& pbm = mesh().boundary();
  label nPoints = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs_, iter)
  {
    label patchI = iter.key();
    nPoints += pbm[patchI].meshPoints().size();
  }
  externalPointEdgePoint::trackingData td(points_);
  // Set initial changed points to all the patch points(if patch present)
  List<externalPointEdgePoint> wallInfo(nPoints);
  labelList wallPoints(nPoints);
  nPoints = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs_, iter)
  {
    label patchI = iter.key();
    // Retrieve the patch now we have its index in patches.
    const labelList& mp = pbm[patchI].meshPoints();
    FOR_ALL(mp, ppI)
    {
      label meshPointI = mp[ppI];
      wallPoints[nPoints] = meshPointI;
      wallInfo[nPoints] = externalPointEdgePoint
      (
        td.points_[meshPointI],
        0.0
      );
      nPoints++;
    }
  }
  // Current info on points
  List<externalPointEdgePoint> allPointInfo(mesh()().nPoints());
  // Current info on edges
  List<externalPointEdgePoint> allEdgeInfo(mesh()().nEdges());
  PointEdgeWave
  <
    externalPointEdgePoint,
    externalPointEdgePoint::trackingData
  > wallCalc
  (
    mesh()(),
    wallPoints,
    wallInfo,
    allPointInfo,
    allEdgeInfo,
    mesh().globalData().nTotalPoints(), // max iterations
    td
  );
  pointScalarField& psf = *this;
  FOR_ALL(allPointInfo, pointI)
  {
    if (allPointInfo[pointI].valid(td))
    {
      psf[pointI] = mousse::sqrt(allPointInfo[pointI].distSqr());
    }
    else
    {
      nUnset_++;
    }
  }
}
