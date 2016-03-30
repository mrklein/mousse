// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "list_ops.hpp"
#include "point.hpp"
#include "field.hpp"


// Member Functions 
template<class Type>
mousse::label mousse::mergePoints
(
  const UList<Type>& points,
  const scalar mergeTol,
  const bool verbose,
  labelList& pointMap,
  const Type& origin
)
{
  Type compareOrigin = origin;
  if (origin == Type::max) {
    if (points.size()) {
      compareOrigin = sum(points)/points.size();
    }
  }
  // Create a old to new point mapping array
  pointMap.setSize(points.size());
  pointMap = -1;
  if (points.empty()) {
    return points.size();
  }
  // We're comparing distance squared to origin first.
  // Say if starting from two close points:
  //     x, y, z
  //     x+mergeTol, y+mergeTol, z+mergeTol
  // Then the magSqr of both will be
  //     x^2+y^2+z^2
  //     x^2+y^2+z^2 + 2*mergeTol*(x+z+y) + mergeTol^2*...
  // so the difference will be 2*mergeTol*(x+y+z)
  const scalar mergeTolSqr = mousse::sqr(scalar(mergeTol));
  // Sort points by magSqr
  const Field<Type> d{points - compareOrigin};
  List<scalar> magSqrD{d.size()};
  FOR_ALL(d, pointI) {
    magSqrD[pointI] = magSqr(d[pointI]);
  }
  labelList order;
  sortedOrder(magSqrD, order);
  Field<scalar> sortedTol{points.size()};
  FOR_ALL(order, sortI) {
    label pointI = order[sortI];
    // Convert to scalar precision
    const point pt
    {
      scalar(d[pointI].x()),
      scalar(d[pointI].y()),
      scalar(d[pointI].z())
    };
    sortedTol[sortI] = 2*mergeTol*(mag(pt.x())+mag(pt.y())+mag(pt.z()));
  }
  label newPointI = 0;
  // Handle 0th point separately (is always unique)
  label pointI = order[0];
  pointMap[pointI] = newPointI++;
  for (label sortI = 1; sortI < order.size(); sortI++) {
    // Get original point index
    label pointI = order[sortI];
    const scalar mag2 = magSqrD[order[sortI]];
    // Convert to scalar precision
    const point pt
    (
      scalar(points[pointI].x()),
      scalar(points[pointI].y()),
      scalar(points[pointI].z())
    );
    // Compare to previous points to find equal one.
    label equalPointI = -1;
    for
    (
      label prevSortI = sortI - 1;
      prevSortI >= 0
      && (mag(magSqrD[order[prevSortI]] - mag2) <= sortedTol[sortI]);
      prevSortI--
    ) {
      label prevPointI = order[prevSortI];
      const point prevPt
      {
        scalar(points[prevPointI].x()),
        scalar(points[prevPointI].y()),
        scalar(points[prevPointI].z())
      };
      if (magSqr(pt - prevPt) <= mergeTolSqr) {
        // Found match.
        equalPointI = prevPointI;
        break;
      }
    }
    if (equalPointI != -1) {
      // Same coordinate as equalPointI. Map to same new point.
      pointMap[pointI] = pointMap[equalPointI];
      if (verbose) {
        Pout << "mousse::mergePoints : Merging points "
          << pointI << " and " << equalPointI
          << " with coordinates:" << points[pointI]
          << " and " << points[equalPointI]
          << endl;
      }
    } else {
      // Differs. Store new point.
      pointMap[pointI] = newPointI++;
    }
  }
  return newPointI;
}


template<class Type>
bool mousse::mergePoints
(
  const UList<Type>& points,
  const scalar mergeTol,
  const bool verbose,
  labelList& pointMap,
  List<Type>& newPoints,
  const Type& origin
)
{
  label nUnique = mergePoints
  (
    points,
    mergeTol,
    verbose,
    pointMap,
    origin
  );
  newPoints.setSize(nUnique);
  FOR_ALL(pointMap, pointI) {
    newPoints[pointMap[pointI]] = points[pointI];
  }
  return (nUnique != points.size());
}
