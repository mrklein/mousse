// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "threshold_cell_faces.hpp"
#include "poly_mesh.hpp"
#include "dynamic_list.hpp"
#include "empty_poly_patch.hpp"
#include "processor_poly_patch.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(thresholdCellFaces, 0);
}
// Private Member Functions 
void mousse::thresholdCellFaces::calculate
(
  const scalarField& field,
  const scalar lowerThreshold,
  const scalar upperThreshold,
  const bool triangulate
)
{
  const labelList& own = mesh_.faceOwner();
  const labelList& nei = mesh_.faceNeighbour();
  const faceList& origFaces = mesh_.faces();
  const pointField& origPoints = mesh_.points();
  const polyBoundaryMesh& bMesh = mesh_.boundaryMesh();
  surfZoneList surfZones(bMesh.size()+1);
  surfZones[0] = surfZone
  (
    "internalMesh",
    0,  // size
    0,  // start
    0   // index
  );
  forAll(bMesh, patchI)
  {
    surfZones[patchI+1] = surfZone
    (
      bMesh[patchI].name(),
      0,        // size
      0,        // start
      patchI+1  // index
    );
  }
  label nFaces = 0;
  label nPoints = 0;
  meshCells_.clear();
  DynamicList<face>  surfFaces(0.5 * mesh_.nFaces());
  DynamicList<label> surfCells(surfFaces.size());
  labelList oldToNewPoints(origPoints.size(), -1);
  // internal faces only
  for (label faceI = 0; faceI < mesh_.nInternalFaces(); ++faceI)
  {
    int side = 0;
    // check lowerThreshold
    if (field[own[faceI]] > lowerThreshold)
    {
      if (field[nei[faceI]] < lowerThreshold)
      {
        side = +1;
      }
    }
    else if (field[nei[faceI]] > lowerThreshold)
    {
      side = -1;
    }
    // check upperThreshold
    if (field[own[faceI]] < upperThreshold)
    {
      if (field[nei[faceI]] > upperThreshold)
      {
        side = +1;
      }
    }
    else if (field[nei[faceI]] < upperThreshold)
    {
      side = -1;
    }
    if (side)
    {
      const face& f = origFaces[faceI];
      forAll(f, fp)
      {
        if (oldToNewPoints[f[fp]] == -1)
        {
          oldToNewPoints[f[fp]] = nPoints++;
        }
      }
      label cellId;
      face  surfFace;
      if (side > 0)
      {
        surfFace = f;
        cellId = own[faceI];
      }
      else
      {
        surfFace = f.reverseFace();
        cellId = nei[faceI];
      }
      if (triangulate)
      {
        label count = surfFace.triangles(origPoints, surfFaces);
        while (count-- > 0)
        {
          surfCells.append(cellId);
        }
      }
      else
      {
        surfFaces.append(surfFace);
        surfCells.append(cellId);
      }
    }
  }
  surfZones[0].size() = surfFaces.size();
  // nothing special for processor patches?
  forAll(bMesh, patchI)
  {
    const polyPatch& p = bMesh[patchI];
    surfZone& zone = surfZones[patchI+1];
    zone.start() = nFaces;
    if
    (
      isA<emptyPolyPatch>(p)
    || (Pstream::parRun() && isA<processorPolyPatch>(p))
    )
    {
      continue;
    }
    label faceI = p.start();
    // patch faces
    forAll(p, localFaceI)
    {
      if
      (
        field[own[faceI]] > lowerThreshold
      && field[own[faceI]] < upperThreshold
      )
      {
        const face& f = origFaces[faceI];
        forAll(f, fp)
        {
          if (oldToNewPoints[f[fp]] == -1)
          {
            oldToNewPoints[f[fp]] = nPoints++;
          }
        }
        label cellId = own[faceI];
        if (triangulate)
        {
          label count = f.triangles(origPoints, surfFaces);
          while (count-- > 0)
          {
            surfCells.append(cellId);
          }
        }
        else
        {
          surfFaces.append(f);
          surfCells.append(cellId);
        }
      }
      ++faceI;
    }
    zone.size() = surfFaces.size() - zone.start();
  }
  surfFaces.shrink();
  surfCells.shrink();
  // renumber
  forAll(surfFaces, faceI)
  {
    inplaceRenumber(oldToNewPoints, surfFaces[faceI]);
  }
  pointField surfPoints(nPoints);
  nPoints = 0;
  forAll(oldToNewPoints, pointI)
  {
    if (oldToNewPoints[pointI] >= 0)
    {
      surfPoints[oldToNewPoints[pointI]] = origPoints[pointI];
      nPoints++;
    }
  }
  surfPoints.setSize(nPoints);
  this->storedPoints().transfer(surfPoints);
  this->storedFaces().transfer(surfFaces);
  this->storedZones().transfer(surfZones);
  meshCells_.transfer(surfCells);
}
// Constructors 
mousse::thresholdCellFaces::thresholdCellFaces
(
  const polyMesh& mesh,
  const scalarField& field,
  const scalar lowerThreshold,
  const scalar upperThreshold,
  const bool triangulate
)
:
  mesh_(mesh)
{
  if (lowerThreshold > upperThreshold)
  {
    WarningIn("thresholdCellFaces::thresholdCellFaces(...)")
      << "lower > upper limit!  "
      << lowerThreshold << " > " << upperThreshold << endl;
  }
  calculate(field, lowerThreshold, upperThreshold, triangulate);
}
