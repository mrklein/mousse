// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bool_list.hpp"
#include "point_hit.hpp"
#include "object_hit.hpp"
#include "band_compression.hpp"


// Member Functions 
template
<
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
template<class ToPatch>
mousse::List<mousse::objectHit>
mousse::PrimitivePatch<Face, FaceList, PointField, PointType>::
projectPoints
(
  const ToPatch& targetPatch,
  const Field<PointType>& projectionDirection,
  const intersection::algorithm alg,
  const intersection::direction dir
) const
{
  // The current patch is slave, i.e. it is being projected onto the target
  if (projectionDirection.size() != nPoints()) {
    FATAL_ERROR_IN
    (
      "PrimitivePatch<Face, FaceList, PointField, PointType>::"
      "projectPoints(const PrimitivePatch& "
      ", const Field<PointType>&) const"
    )
    << "Projection direction field does not correspond to "
    << "patch points." << endl
    << "Size: " << projectionDirection.size()
    << " Number of points: " << nPoints()
    << abort(FatalError);
  }
  const labelList& slavePointOrder = localPointOrder();
  const labelList& slaveMeshPoints = meshPoints();
  // Result
  List<objectHit> result{nPoints()};
  const labelListList& masterFaceFaces = targetPatch.faceFaces();
  const ToPatch& masterFaces = targetPatch;
  const Field<PointType>& masterPoints = targetPatch.points();
  // Estimate face centre of target side
  Field<PointType> masterFaceCentres{targetPatch.size()};
  FOR_ALL(masterFaceCentres, faceI) {
    masterFaceCentres[faceI] =
      average(masterFaces[faceI].points(masterPoints));
  }
  // Algorithm:
  // Loop through all points of the slave side. For every point find the
  // radius for the current contact face. If the contact point falls inside
  // the face and the radius is smaller than for all neighbouring faces,
  // the contact is found. If not, visit the neighbour closest to the
  // calculated contact point. If a single master face is visited more than
  // twice, initiate n-squared search.
  label curFace = 0;
  label nNSquaredSearches = 0;
  FOR_ALL(slavePointOrder, pointI) {
    // Pick up slave point and direction
    const label curLocalPointLabel = slavePointOrder[pointI];
    const PointType& curPoint =
      points_[slaveMeshPoints[curLocalPointLabel]];
    const PointType& curProjectionDir =
      projectionDirection[curLocalPointLabel];
    bool closer;
    boolList visitedTargetFace{targetPatch.size(), false};
    bool doNSquaredSearch = false;
    bool foundEligible = false;
    scalar sqrDistance = GREAT;
    // Force the full search for the first point to ensure good
    // starting face
    if (pointI == 0) {
      doNSquaredSearch = true;
    } else {
      do {
        closer = false;
        doNSquaredSearch = false;
        // Calculate intersection with curFace
        PointHit<PointType> curHit =
          masterFaces[curFace].ray
          (
            curPoint,
            curProjectionDir,
            masterPoints,
            alg,
            dir
          );
        visitedTargetFace[curFace] = true;
        if (curHit.hit()) {
          result[curLocalPointLabel] = objectHit(true, curFace);
          break;
        } else {
          // If a new miss is eligible, it is closer than
          // any previous eligible miss (due to surface walk)
          // Only grab the miss if it is eligible
          if (curHit.eligibleMiss()) {
            foundEligible = true;
            result[curLocalPointLabel] = objectHit(false, curFace);
          }
          // Find the next likely face for intersection
          // Calculate the miss point on the plane of the
          // face.  This is cooked (illogical!) for fastest
          // surface walk.
          //
          PointType missPlanePoint =
            curPoint + curProjectionDir*curHit.distance();
          const labelList& masterNbrs = masterFaceFaces[curFace];
          sqrDistance =
            magSqr(missPlanePoint - masterFaceCentres[curFace]);
          FOR_ALL(masterNbrs, nbrI) {
            if (magSqr(missPlanePoint - masterFaceCentres[masterNbrs[nbrI]])
                <= sqrDistance) {
              closer = true;
              curFace = masterNbrs[nbrI];
            }
          }
          if (visitedTargetFace[curFace]) {
            // This face has already been visited.
            // Execute n-squared search
            doNSquaredSearch = true;
            break;
          }
        }
        if (debug)
          Info<< ".";
      } while (closer);
    }
    if (doNSquaredSearch || !foundEligible) {
      nNSquaredSearches++;
      if (debug) {
        Info<< "p " << curLocalPointLabel << ": ";
      }
      result[curLocalPointLabel] = objectHit(false, -1);
      scalar minDistance = GREAT;
      FOR_ALL(masterFaces, faceI) {
        PointHit<PointType> curHit =
          masterFaces[faceI].ray
          (
            curPoint,
            curProjectionDir,
            masterPoints,
            alg,
            dir
          );
        if (curHit.hit()) {
          result[curLocalPointLabel] = objectHit(true, faceI);
          curFace = faceI;
          break;
        } else if (curHit.eligibleMiss()) {
          // Calculate min distance
          scalar missDist =
            mousse::mag(curHit.missPoint() - curPoint);
          if (missDist < minDistance) {
            minDistance = missDist;
            result[curLocalPointLabel] = objectHit(false, faceI);
            curFace = faceI;
          }
        }
      }
      if (debug) {
        Info << result[curLocalPointLabel] << nl;
      }
    } else {
      if (debug)
        Info << "x";
    }
  }
  if (debug) {
    Info << nl << "Executed " << nNSquaredSearches
      << " n-squared searches out of total of "
      << nPoints() << endl;
  }

  return result;
}


template
<
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
template<class ToPatch>
mousse::List<mousse::objectHit>
mousse::PrimitivePatch<Face, FaceList, PointField, PointType>::
projectFaceCentres
(
  const ToPatch& targetPatch,
  const Field<PointType>& projectionDirection,
  const intersection::algorithm alg,
  const intersection::direction dir
) const
{
  // The current patch is slave, i.e. it is being projected onto the target
  if (projectionDirection.size() != this->size()) {
    FATAL_ERROR_IN
    (
      "labelList PrimitivePatch<Face, FaceList, PointField, PointType>::"
      "projectFaceCentres(const PrimitivePatch& "
      ", const Field<PointType>&) const"
    )
    << "Projection direction field does not correspond to patch faces."
    << endl << "Size: " << projectionDirection.size()
    << " Number of points: " << this->size()
    << abort(FatalError);
  }
  labelList slaveFaceOrder = bandCompression(faceFaces());
  // calculate master face centres
  Field<PointType> masterFaceCentres{targetPatch.size()};
  const labelListList& masterFaceFaces = targetPatch.faceFaces();
  const ToPatch& masterFaces = targetPatch;
  const typename ToPatch::PointFieldType& masterPoints = targetPatch.points();
  FOR_ALL(masterFaceCentres, faceI) {
    masterFaceCentres[faceI] =
      masterFaces[faceI].centre(masterPoints);
  }
  // Result
  List<objectHit> result{this->size()};
  const PrimitivePatch<Face, FaceList, PointField, PointType>& slaveFaces =
    *this;
  const PointField& slaveGlobalPoints = points();
  // Algorithm:
  // Loop through all points of the slave side. For every point find the
  // radius for the current contact face. If the contact point falls inside
  // the face and the radius is smaller than for all neighbouring faces,
  // the contact is found. If not, visit the neighbour closest to the
  // calculated contact point. If a single master face is visited more than
  // twice, initiate n-squared search.
  label curFace = 0;
  label nNSquaredSearches = 0;
  FOR_ALL(slaveFaceOrder, faceI) {
    // pick up slave point and direction
    const label curLocalFaceLabel = slaveFaceOrder[faceI];
    const point& curFaceCentre =
      slaveFaces[curLocalFaceLabel].centre(slaveGlobalPoints);
    const vector& curProjectionDir =
      projectionDirection[curLocalFaceLabel];
    bool closer;
    boolList visitedTargetFace{targetPatch.size(), false};
    bool doNSquaredSearch = false;
    bool foundEligible = false;
    scalar sqrDistance = GREAT;
    // Force the full search for the first point to ensure good
    // starting face
    if (faceI == 0) {
      doNSquaredSearch = true;
    } else {
      do {
        closer = false;
        doNSquaredSearch = false;
        // Calculate intersection with curFace
        PointHit<PointType> curHit =
          masterFaces[curFace].ray
          (
            curFaceCentre,
            curProjectionDir,
            masterPoints,
            alg,
            dir
          );
        visitedTargetFace[curFace] = true;
        if (curHit.hit()) {
          result[curLocalFaceLabel] = objectHit(true, curFace);
          break;
        } else {
          // If a new miss is eligible, it is closer than
          // any previous eligible miss (due to surface walk)
          // Only grab the miss if it is eligible
          if (curHit.eligibleMiss()) {
            foundEligible = true;
            result[curLocalFaceLabel] = objectHit(false, curFace);
          }
          // Find the next likely face for intersection
          // Calculate the miss point.  This is
          // cooked (illogical!) for fastest surface walk.
          //
          PointType missPlanePoint =
            curFaceCentre + curProjectionDir*curHit.distance();
          sqrDistance =
            magSqr(missPlanePoint - masterFaceCentres[curFace]);
          const labelList& masterNbrs = masterFaceFaces[curFace];
          FOR_ALL(masterNbrs, nbrI) {
            if (magSqr
                (
                  missPlanePoint
                  - masterFaceCentres[masterNbrs[nbrI]]
                )
                <= sqrDistance) {
              closer = true;
              curFace = masterNbrs[nbrI];
            }
          }
          if (visitedTargetFace[curFace]) {
            // This face has already been visited.
            // Execute n-squared search
            doNSquaredSearch = true;
            break;
          }
        }
        if (debug)
          Info<< ".";
      } while (closer);
    }
    if (doNSquaredSearch || !foundEligible) {
      nNSquaredSearches++;
      if (debug) {
        Info << "p " << curLocalFaceLabel << ": ";
      }
      result[curLocalFaceLabel] = objectHit(false, -1);
      scalar minDistance = GREAT;
      FOR_ALL(masterFaces, faceI) {
        PointHit<PointType> curHit =
          masterFaces[faceI].ray
          (
            curFaceCentre,
            curProjectionDir,
            masterPoints,
            alg,
            dir
          );
        if (curHit.hit()) {
          result[curLocalFaceLabel] = objectHit(true, faceI);
          curFace = faceI;
          break;
        } else if (curHit.eligibleMiss()) {
          // Calculate min distance
          scalar missDist =
            mousse::mag(curHit.missPoint() - curFaceCentre);
          if (missDist < minDistance) {
            minDistance = missDist;
            result[curLocalFaceLabel] = objectHit(false, faceI);
            curFace = faceI;
          }
        }
      }
      if (debug) {
        Info << result[curLocalFaceLabel] << nl;
      }
    } else {
      if (debug)
        Info<< "x";
    }
  }
  if (debug) {
    Info << nl << "Executed " << nNSquaredSearches
      << " n-squared searches out of total of "
      << this->size() << endl;
  }
  return result;
}

