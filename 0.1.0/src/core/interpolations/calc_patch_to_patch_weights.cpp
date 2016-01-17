// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_to_patch_interpolation.hpp"
#include "object_hit.hpp"
#include "point_hit.hpp"

namespace mousse
{

// Static Data Members
template<class FromPatch, class ToPatch>
scalar PatchToPatchInterpolation<FromPatch, ToPatch>::projectionTol_ = 0.05;

// Private Member Functions 
template<class FromPatch, class ToPatch>
void PatchToPatchInterpolation<FromPatch, ToPatch>::calcPointAddressing() const
{
  // Calculate pointWeights
  pointWeightsPtr_ = new FieldField<Field, scalar>{toPatch_.nPoints()};
  FieldField<Field, scalar>& pointWeights = *pointWeightsPtr_;
  pointDistancePtr_ = new scalarField{toPatch_.nPoints(), GREAT};
  scalarField& pointDistance = *pointDistancePtr_;
  const pointField& fromPatchPoints = fromPatch_.localPoints();
  const List<typename FromPatch::FaceType>& fromPatchFaces =
    fromPatch_.localFaces();
  const pointField& toPatchPoints = toPatch_.localPoints();
  const vectorField& projectionDirection = toPatch_.pointNormals();
  const edgeList& toPatchEdges = toPatch_.edges();
  const labelListList& toPatchPointEdges = toPatch_.pointEdges();
  if (debug)
  {
    Info<< "projecting points" << endl;
  }
  List<objectHit> proj =
    toPatch_.projectPoints(fromPatch_, projectionDirection, alg_, dir_);
  pointAddressingPtr_ = new labelList{proj.size(), -1};
  labelList& pointAddressing = *pointAddressingPtr_;
  bool doWeights = false;
  FOR_ALL(pointAddressing, pointI)
  {
    doWeights = false;
    const typename FromPatch::FaceType& hitFace =
      fromPatchFaces[proj[pointI].hitObject()];
    point hitPoint = point::zero;
    if (proj[pointI].hit())
    {
      // A hit exists
      doWeights = true;
      pointAddressing[pointI] = proj[pointI].hitObject();
      pointHit curHit =
        hitFace.ray
        (
          toPatchPoints[pointI],
          projectionDirection[pointI],
          fromPatchPoints,
          alg_,
          dir_
        );
      // Grab distance to target
      if (dir_ == intersection::CONTACT_SPHERE)
      {
        pointDistance[pointI] =
          hitFace.contactSphereDiameter
          (
            toPatchPoints[pointI],
            projectionDirection[pointI],
            fromPatchPoints
          );
      }
      else
      {
        pointDistance[pointI] = curHit.distance();
      }
      // Grab hit point
      hitPoint = curHit.hitPoint();
    }
    else if (projectionTol_ > SMALL)
    {
      // Check for a near miss
      pointHit ph =
        hitFace.ray
        (
          toPatchPoints[pointI],
          projectionDirection[pointI],
          fromPatchPoints,
          alg_,
          dir_
        );
      scalar dist =
        mousse::mag
        (
          toPatchPoints[pointI]
          + projectionDirection[pointI]*ph.distance()
          - ph.missPoint()
        );
      // Calculate the local tolerance
      scalar minEdgeLength = GREAT;
      // Do shortest edge of hit object
      edgeList hitFaceEdges =
        fromPatchFaces[proj[pointI].hitObject()].edges();
      FOR_ALL(hitFaceEdges, edgeI)
      {
        minEdgeLength =
          min
          (
            minEdgeLength,
            hitFaceEdges[edgeI].mag(fromPatchPoints)
          );
      }
      const labelList& curEdges = toPatchPointEdges[pointI];
      FOR_ALL(curEdges, edgeI)
      {
        minEdgeLength =
          min
          (
            minEdgeLength,
            toPatchEdges[curEdges[edgeI]].mag(toPatchPoints)
          );
      }
      if (dist < minEdgeLength*projectionTol_)
      {
        // This point is being corrected
        doWeights = true;
        pointAddressing[pointI] = proj[pointI].hitObject();
        // Grab nearest point on face as hit point
        hitPoint = ph.missPoint();
        // Grab distance to target
        if (dir_ == intersection::CONTACT_SPHERE)
        {
          pointDistance[pointI] =
            hitFace.contactSphereDiameter
            (
              toPatchPoints[pointI],
              projectionDirection[pointI],
              fromPatchPoints
            );
        }
        else
        {
          pointDistance[pointI] =
            (
              projectionDirection[pointI]
              /mag(projectionDirection[pointI])
            )
           & (hitPoint - toPatchPoints[pointI]);
        }
      }
    }
    if (doWeights)
    {
      // Set interpolation pointWeights
      pointWeights.set(pointI, new scalarField(hitFace.size()));
      pointField hitFacePoints = hitFace.points(fromPatchPoints);
      FOR_ALL(hitFacePoints, masterPointI)
      {
        pointWeights[pointI][masterPointI] =
          1.0/
          (
            mag
            (
              hitFacePoints[masterPointI]
             - hitPoint
            )
           + VSMALL
          );
      }
      pointWeights[pointI] /= sum(pointWeights[pointI]);
    }
    else
    {
      pointWeights.set(pointI, new scalarField(0));
    }
  }
}


template<class FromPatch, class ToPatch>
void PatchToPatchInterpolation<FromPatch, ToPatch>::calcFaceAddressing() const
{
  faceWeightsPtr_ = new FieldField<Field, scalar>{toPatch_.size()};
  FieldField<Field, scalar>& faceWeights = *faceWeightsPtr_;
  faceDistancePtr_ = new scalarField{toPatch_.size(), GREAT};
  scalarField& faceDistance = *faceDistancePtr_;
  if (debug)
  {
    Info<< "projecting face centres" << endl;
  }
  const pointField& fromPatchPoints = fromPatch_.points();
  const typename FromPatch::FaceListType& fromPatchFaces = fromPatch_;
  const labelListList& fromPatchFaceFaces = fromPatch_.faceFaces();
  vectorField fromPatchFaceCentres{fromPatchFaces.size()};
  FOR_ALL(fromPatchFaceCentres, faceI)
  {
    fromPatchFaceCentres[faceI] =
      fromPatchFaces[faceI].centre(fromPatchPoints);
  }
  const pointField& toPatchPoints = toPatch_.points();
  const typename ToPatch::FaceListType& toPatchFaces = toPatch_;
  const vectorField& projectionDirection = toPatch_.faceNormals();
  List<objectHit> proj =
    toPatch_.projectFaceCentres
    (
      fromPatch_,
      projectionDirection,
      alg_,
      dir_
    );
  faceAddressingPtr_ = new labelList{proj.size(), -1};
  labelList& faceAddressing = *faceAddressingPtr_;
  FOR_ALL(faceAddressing, faceI)
  {
    if (proj[faceI].hit())
    {
      // A hit exists
      faceAddressing[faceI] = proj[faceI].hitObject();
      const typename FromPatch::FaceType& hitFace =
        fromPatchFaces[faceAddressing[faceI]];
      pointHit curHit =
        hitFace.ray
        (
          toPatchFaces[faceI].centre(toPatchPoints),
          projectionDirection[faceI],
          fromPatchPoints,
          alg_,
          dir_
        );
      // grab distance to target
      faceDistance[faceI] = curHit.distance();
      // grab face centre of the hit face
      const point& hitFaceCentre =
        fromPatchFaceCentres[faceAddressing[faceI]];
      // grab neighbours of hit face
      const labelList& neighbours =
        fromPatchFaceFaces[faceAddressing[faceI]];
      scalar m = mag(curHit.hitPoint() - hitFaceCentre);
      if (m < directHitTol /* Direct hit */ || neighbours.empty())
      {
        faceWeights.set(faceI, new scalarField(1));
        faceWeights[faceI][0] = 1.0;
      }
      else
      {
        // set interpolation faceWeights
        // The first coefficient corresponds to the centre face.
        // The rest is ordered in the same way as the faceFaces list.
        faceWeights.set(faceI, new scalarField(neighbours.size() + 1));
        faceWeights[faceI][0] = 1.0/m;
        FOR_ALL(neighbours, nI)
        {
          faceWeights[faceI][nI + 1] =
          1.0/
          (
            mag
            (
              fromPatchFaceCentres[neighbours[nI]]
             - curHit.hitPoint()
            )
           + VSMALL
          );
        }
      }
      faceWeights[faceI] /= sum(faceWeights[faceI]);
    }
    else
    {
      faceWeights.set(faceI, new scalarField(0));
    }
  }
}

}  // namespace mousse
