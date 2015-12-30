// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inverse_point_distance_diffusivity.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "hash_set.hpp"
#include "point_edge_point.hpp"
#include "point_edge_wave.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(inversePointDistanceDiffusivity, 0);
  addToRunTimeSelectionTable
  (
    motionDiffusivity,
    inversePointDistanceDiffusivity,
    Istream
  );
}
// Constructors 
mousse::inversePointDistanceDiffusivity::inversePointDistanceDiffusivity
(
  const fvMesh& mesh,
  Istream& mdData
)
:
  uniformDiffusivity(mesh, mdData),
  patchNames_(mdData)
{
  correct();
}
// Destructor 
mousse::inversePointDistanceDiffusivity::~inversePointDistanceDiffusivity()
{}
// Member Functions 
void mousse::inversePointDistanceDiffusivity::correct()
{
  const polyBoundaryMesh& bdry = mesh().boundaryMesh();
  labelHashSet patchSet(bdry.patchSet(patchNames_));
  label nPatchEdges = 0;
  forAllConstIter(labelHashSet, patchSet, iter)
  {
    nPatchEdges += bdry[iter.key()].nEdges();
  }
  // Distance to wall on points and edges.
  List<pointEdgePoint> pointWallDist(mesh().nPoints());
  List<pointEdgePoint> edgeWallDist(mesh().nEdges());
  int dummyTrackData = 0;
  {
    // Seeds
    List<pointEdgePoint> seedInfo(nPatchEdges);
    labelList seedPoints(nPatchEdges);
    nPatchEdges = 0;
    forAllConstIter(labelHashSet, patchSet, iter)
    {
      const polyPatch& patch = bdry[iter.key()];
      const labelList& meshPoints = patch.meshPoints();
      forAll(meshPoints, i)
      {
        label pointI = meshPoints[i];
        if (!pointWallDist[pointI].valid(dummyTrackData))
        {
          // Not yet seeded
          seedInfo[nPatchEdges] = pointEdgePoint
          (
            mesh().points()[pointI],
            0.0
          );
          seedPoints[nPatchEdges] = pointI;
          pointWallDist[pointI] = seedInfo[nPatchEdges];
          nPatchEdges++;
        }
      }
    }
    seedInfo.setSize(nPatchEdges);
    seedPoints.setSize(nPatchEdges);
    // Do calculations
    PointEdgeWave<pointEdgePoint> waveInfo
    (
      mesh(),
      seedPoints,
      seedInfo,
      pointWallDist,
      edgeWallDist,
      mesh().globalData().nTotalPoints(),// max iterations
      dummyTrackData
    );
  }
  for (label faceI=0; faceI<mesh().nInternalFaces(); faceI++)
  {
    const face& f = mesh().faces()[faceI];
    scalar dist = 0;
    forAll(f, fp)
    {
      dist += sqrt(pointWallDist[f[fp]].distSqr());
    }
    dist /= f.size();
    faceDiffusivity_[faceI] = 1.0/dist;
  }
  forAll(faceDiffusivity_.boundaryField(), patchI)
  {
    fvsPatchScalarField& bfld = faceDiffusivity_.boundaryField()[patchI];
    if (patchSet.found(patchI))
    {
      const labelUList& faceCells = bfld.patch().faceCells();
      forAll(bfld, i)
      {
        const cell& ownFaces = mesh().cells()[faceCells[i]];
        labelHashSet cPoints(4*ownFaces.size());
        scalar dist = 0;
        forAll(ownFaces, ownFaceI)
        {
          const face& f = mesh().faces()[ownFaces[ownFaceI]];
          forAll(f, fp)
          {
            if (cPoints.insert(f[fp]))
            {
              dist += sqrt(pointWallDist[f[fp]].distSqr());
            }
          }
        }
        dist /= cPoints.size();
        bfld[i] = 1.0/dist;
      }
    }
    else
    {
      const label start = bfld.patch().start();
      forAll(bfld, i)
      {
        const face& f = mesh().faces()[start+i];
        scalar dist = 0;
        forAll(f, fp)
        {
          dist += sqrt(pointWallDist[f[fp]].distSqr());
        }
        dist /= f.size();
        bfld[i] = 1.0/dist;
      }
    }
  }
}
