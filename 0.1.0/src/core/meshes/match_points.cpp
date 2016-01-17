// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "match_points.hpp"
#include "sortable_list.hpp"
#include "list_ops.hpp"
#include "iostreams.hpp"

// Member Functions
bool mousse::matchPoints
(
  const UList<point>& pts0,
  const UList<point>& pts1,
  const UList<scalar>& matchDistances,
  const bool verbose,
  labelList& from0To1,
  const point& origin
)
{
  from0To1.setSize(pts0.size());
  from0To1 = -1;
  bool fullMatch = true;
  point compareOrigin = origin;
  if (origin == point(VGREAT, VGREAT, VGREAT))
  {
    if (pts1.size())
    {
      compareOrigin = sum(pts1)/pts1.size();
    }
  }
  SortableList<scalar> pts0MagSqr(magSqr(pts0 - compareOrigin));
  SortableList<scalar> pts1MagSqr(magSqr(pts1 - compareOrigin));
  FOR_ALL(pts0MagSqr, i)
  {
    scalar dist0 = pts0MagSqr[i];
    label face0I = pts0MagSqr.indices()[i];
    scalar matchDist = matchDistances[face0I];
    label startI = findLower(pts1MagSqr, 0.99999*dist0 - 2*matchDist);
    if (startI == -1)
    {
      startI = 0;
    }
    // Go through range of equal mag and find nearest vector.
    scalar minDistSqr = VGREAT;
    label minFaceI = -1;
    for
    (
      label j = startI;
      (
        (j < pts1MagSqr.size())
      && (pts1MagSqr[j] < 1.00001*dist0 + 2*matchDist)
      );
      j++
    )
    {
      label faceI = pts1MagSqr.indices()[j];
      // Compare actual vectors
      scalar distSqr = magSqr(pts0[face0I] - pts1[faceI]);
      if (distSqr <= sqr(matchDist) && distSqr < minDistSqr)
      {
        minDistSqr = distSqr;
        minFaceI = faceI;
      }
    }
    if (minFaceI == -1)
    {
      fullMatch = false;
      if (verbose)
      {
        Pout<< "Cannot find point in pts1 matching point " << face0I
          << " coord:" << pts0[face0I]
          << " in pts0 when using tolerance " << matchDist << endl;
        // Go through range of equal mag and find equal vector.
        Pout<< "Searching started from:" << startI << " in pts1"
          << endl;
        for
        (
          label j = startI;
          (
            (j < pts1MagSqr.size())
          && (pts1MagSqr[j] < 1.00001*dist0 + 2*matchDist)
          );
          j++
        )
        {
          label faceI = pts1MagSqr.indices()[j];
          Pout<< "    Compared coord: " << pts1[faceI]
            << " at index " << j
            << " with difference to point "
            << mag(pts1[faceI] - pts0[face0I]) << endl;
        }
      }
    }
    from0To1[face0I] = minFaceI;
  }
  return fullMatch;
}
bool mousse::matchPoints
(
  const UList<point>& pts0,
  const UList<point>& pts1,
  const UList<point>& pts0Dir,
  const UList<point>& pts1Dir,
  const UList<scalar>& matchDistances,
  const bool verbose,
  labelList& from0To1,
  const point& origin
)
{
  from0To1.setSize(pts0.size());
  from0To1 = -1;
  bool fullMatch = true;
  point compareOrigin = origin;
  if (origin == point(VGREAT, VGREAT, VGREAT))
  {
    if (pts1.size())
    {
      compareOrigin = sum(pts1)/pts1.size();
    }
  }
  SortableList<scalar> pts0MagSqr(magSqr(pts0 - compareOrigin));
  SortableList<scalar> pts1MagSqr(magSqr(pts1 - compareOrigin));
  FOR_ALL(pts0MagSqr, i)
  {
    scalar dist0 = pts0MagSqr[i];
    label face0I = pts0MagSqr.indices()[i];
    scalar matchDist = matchDistances[face0I];
    label startI = findLower(pts1MagSqr, 0.99999*dist0 - 2*matchDist);
    if (startI == -1)
    {
      startI = 0;
    }
    // Go through range of equal mag and find nearest vector.
    scalar minDistSqr = VGREAT;
    scalar minDistNorm = 0;
    label minFaceI = -1;
    for
    (
      label j = startI;
      (
        (j < pts1MagSqr.size())
      && (pts1MagSqr[j] < 1.00001*dist0 + 2*matchDist)
      );
      j++
    )
    {
      label faceI = pts1MagSqr.indices()[j];
      // Compare actual vectors
      scalar distSqr = magSqr(pts0[face0I] - pts1[faceI]);
      scalar distNorm = (pts0Dir[face0I] & pts1Dir[faceI]);
      if
      (
        magSqr(pts0Dir[face0I]) < sqr(SMALL)
      && magSqr(pts1Dir[faceI]) < sqr(SMALL)
      )
      {
        distNorm = -1;
      }
      if (distSqr <= sqr(matchDist) && distSqr < minDistSqr)
      {
        // Check that the normals point in equal and opposite directions
        if (distNorm < minDistNorm)
        {
          minDistNorm = distNorm;
          minDistSqr = distSqr;
          minFaceI = faceI;
        }
      }
    }
    if (minFaceI == -1)
    {
      fullMatch = false;
      if (verbose)
      {
        Pout<< "Cannot find point in pts1 matching point " << face0I
          << " coord:" << pts0[face0I]
          << " in pts0 when using tolerance " << matchDist << endl;
        // Go through range of equal mag and find equal vector.
        Pout<< "Searching started from:" << startI << " in pts1"
          << endl;
        for
        (
          label j = startI;
          (
            (j < pts1MagSqr.size())
          && (pts1MagSqr[j] < 1.00001*dist0 + 2*matchDist)
          );
          j++
        )
        {
          label faceI = pts1MagSqr.indices()[j];
          Pout<< "    Compared coord: " << pts1[faceI]
            << " at index " << j
            << " with difference to point "
            << mag(pts1[faceI] - pts0[face0I]) << endl;
        }
      }
    }
    from0To1[face0I] = minFaceI;
  }
  return fullMatch;
}
