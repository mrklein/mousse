// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_surfaces_queries.hpp"
#include "list_ops.hpp"
#include "ofstream.hpp"
#include "mesh_tools.hpp"
#include "dynamic_field.hpp"
#include "ptr_list.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(searchableSurfacesQueries, 0);

}


// Private Member Functions
mousse::pointIndexHit mousse::searchableSurfacesQueries::tempFindNearest
(
  const searchableSurface& surf,
  const point& pt,
  const scalar initDistSqr
)
{
  pointField onePoint{1, pt};
  scalarField oneDist{1, initDistSqr};
  List<pointIndexHit> oneHit{1};
  surf.findNearest(onePoint, oneDist, oneHit);
  return oneHit[0];
}


// Calculate sum of distance to surfaces.
mousse::scalar mousse::searchableSurfacesQueries::sumDistSqr
(
  const PtrList<searchableSurface>& allSurfaces,
  const labelList& surfacesToTest,
  const scalar initDistSqr,
  const point& pt
)
{
  scalar sum = 0;
  FOR_ALL(surfacesToTest, testI) {
    label surfI = surfacesToTest[testI];
    pointIndexHit hit{tempFindNearest(allSurfaces[surfI], pt, initDistSqr)};
    // Note: make it fall over if not hit.
    sum += magSqr(hit.hitPoint()-pt);
  }
  return sum;
}


// Reflects the point furthest away around the triangle centre by a factor fac.
// (triangle centre is the average of all points but the ihi. pSum is running
//  sum of all points)
mousse::scalar mousse::searchableSurfacesQueries::tryMorphTet
(
  const PtrList<searchableSurface>& allSurfaces,
  const labelList& surfacesToTest,
  const scalar initDistSqr,
  List<vector>& p,
  List<scalar>& y,
  vector& pSum,
  const label ihi,
  const scalar fac
)
{
  scalar fac1 = (1.0-fac)/vector::nComponents;
  scalar fac2 = fac1-fac;
  vector ptry = pSum*fac1-p[ihi]*fac2;
  scalar ytry = sumDistSqr(allSurfaces, surfacesToTest, initDistSqr, ptry);
  if (ytry < y[ihi]) {
    y[ihi] = ytry;
    pSum += ptry - p[ihi];
    p[ihi] = ptry;
  }
  return ytry;
}


bool mousse::searchableSurfacesQueries::morphTet
(
  const PtrList<searchableSurface>& allSurfaces,
  const labelList& surfacesToTest,
  const scalar /*initDistSqr*/,
  const scalar convergenceDistSqr,
  const label maxIter,
  List<vector>& p,
  List<scalar>& y
)
{
  vector pSum = sum(p);
  autoPtr<OFstream> str;
  label vertI = 0;
  if (debug) {
    wordList names{surfacesToTest.size()};
    FOR_ALL(surfacesToTest, i) {
      names[i] = allSurfaces[surfacesToTest[i]].name();
    }
    Pout << "searchableSurfacesQueries::morphTet : intersection of "
      << names << " starting from points:" << p << endl;
    str.reset(new OFstream{"track.obj"});
    meshTools::writeOBJ(str(), p[0]);
    vertI++;
  }
  for (label iter = 0; iter < maxIter; iter++) {
    // Get the indices of lowest, highest and second-highest values.
    label ilo, ihi, inhi;

    {
      labelList sortedIndices;
      sortedOrder(y, sortedIndices);
      ilo  = sortedIndices[0];
      ihi  = sortedIndices[sortedIndices.size()-1];
      inhi = sortedIndices[sortedIndices.size()-2];
    }

    if (debug) {
      Pout << "Iteration:" << iter
        << " lowest:" << y[ilo] << " highest:" << y[ihi]
        << " points:" << p << endl;
      meshTools::writeOBJ(str(), p[ilo]);
      vertI++;
      str()<< "l " << vertI-1 << ' ' << vertI << nl;
    }
    if (y[ihi] < convergenceDistSqr) {
      // Get point on 0th surface.
      Swap(p[0], p[ilo]);
      Swap(y[0], y[ilo]);
      return true;
    }
    // Reflection: point furthest away gets reflected.
    scalar ytry = tryMorphTet
    (
      allSurfaces,
      surfacesToTest,
      10*y[ihi],             // search box.
      p,
      y,
      pSum,
      ihi,
      -1.0
    );
    if (ytry <= y[ilo]) {
      // If in right direction (y lower) expand by two.
      ytry = tryMorphTet
      (
        allSurfaces,
        surfacesToTest,
        10*y[ihi],
        p,
        y,
        pSum,
        ihi,
        2.0
      );
    } else if (ytry >= y[inhi]) {
      // If inside tet try contraction.
      scalar ysave = y[ihi];
      ytry = tryMorphTet
      (
        allSurfaces,
        surfacesToTest,
        10*y[ihi],
        p,
        y,
        pSum,
        ihi,
        0.5
      );
      if (ytry >= ysave) {
        // Contract around lowest point.
        FOR_ALL(p, i) {
          if (i != ilo) {
            p[i] = 0.5*(p[i] + p[ilo]);
            y[i] = sumDistSqr(allSurfaces, surfacesToTest, y[ihi], p[i]);
          }
        }
        pSum = sum(p);
      }
    }
  }
  if (debug) {
    meshTools::writeOBJ(str(), p[0]);
    vertI++;
    str() << "l " << vertI-1 << ' ' << vertI << nl;
  }
  // Failure to converge. Return best guess so far.
  label ilo = findMin(y);
  Swap(p[0], p[ilo]);
  Swap(y[0], y[ilo]);
  return false;
}


// Given current set of hits (allSurfaces, allInfo) merge in those coming from
// surface surfI.
void mousse::searchableSurfacesQueries::mergeHits
(
  const point& start,
  const label testI,                    // index of surface
  const List<pointIndexHit>& surfHits,  // hits on surface
  labelList& allSurfaces,
  List<pointIndexHit>& allInfo,
  scalarList& allDistSqr
)
{
  // Precalculate distances
  scalarList surfDistSqr{surfHits.size()};
  FOR_ALL(surfHits, i) {
    surfDistSqr[i] = magSqr(surfHits[i].hitPoint() - start);
  }
  FOR_ALL(surfDistSqr, i) {
    label index = findLower(allDistSqr, surfDistSqr[i]);
    // Check if equal to lower.
    if (index >= 0) {
      // Same. Do not count.
    } else {
      // Check if equal to higher
      label next = index + 1;
      if (next < allDistSqr.size()) {
      } else {
        // Insert after index
        label sz = allSurfaces.size();
        allSurfaces.setSize(sz+1);
        allInfo.setSize(allSurfaces.size());
        allDistSqr.setSize(allSurfaces.size());
        // Make space.
        for (label j = sz-1; j > index; --j) {
          allSurfaces[j+1] = allSurfaces[j];
          allInfo[j+1] = allInfo[j];
          allDistSqr[j+1] = allDistSqr[j];
        }
        // Insert new value
        allSurfaces[index+1] = testI;
        allInfo[index+1] = surfHits[i];
        allDistSqr[index+1] = surfDistSqr[i];
      }
    }
  }
}


// Member Functions

// Find any intersection
void mousse::searchableSurfacesQueries::findAnyIntersection
(
  const PtrList<searchableSurface>& allSurfaces,
  const labelList& surfacesToTest,
  const pointField& start,
  const pointField& end,
  labelList& hitSurfaces,
  List<pointIndexHit>& hitInfo
)
{
  hitSurfaces.setSize(start.size());
  hitSurfaces = -1;
  hitInfo.setSize(start.size());
  // Work arrays
  labelList hitMap{identity(start.size())};
  pointField p0{start};
  pointField p1{end};
  List<pointIndexHit> intersectInfo{start.size()};
  FOR_ALL(surfacesToTest, testI) {
    // Do synchronised call to all surfaces.
    allSurfaces[surfacesToTest[testI]].findLineAny(p0, p1, intersectInfo);
    // Copy all hits into arguments, continue with misses
    label newI = 0;
    FOR_ALL(intersectInfo, i) {
      if (intersectInfo[i].hit()) {
        hitInfo[hitMap[i]] = intersectInfo[i];
        hitSurfaces[hitMap[i]] = testI;
      } else {
        if (i != newI) {
          hitMap[newI] = hitMap[i];
          p0[newI] = p0[i];
          p1[newI] = p1[i];
        }
        newI++;
      }
    }
    // All done? Note that this decision should be synchronised
    if (newI == 0) {
      break;
    }
    // Trim and continue
    hitMap.setSize(newI);
    p0.setSize(newI);
    p1.setSize(newI);
    intersectInfo.setSize(newI);
  }
}


void mousse::searchableSurfacesQueries::findAllIntersections
(
  const PtrList<searchableSurface>& allSurfaces,
  const labelList& surfacesToTest,
  const pointField& start,
  const pointField& end,
  labelListList& hitSurfaces,
  List<List<pointIndexHit> >& hitInfo
)
{
  // Note: maybe move the single-surface all intersections test into
  // searchable surface? Some of the tolerance issues might be
  // lessened.
  // 2. Currently calling searchableSurface::findLine with start==end
  //    is expected to find no intersection. Problem if it does.
  hitSurfaces.setSize(start.size());
  hitInfo.setSize(start.size());
  if (surfacesToTest.empty()) {
    return;
  }
  // Test first surface
  allSurfaces[surfacesToTest[0]].findLineAll(start, end, hitInfo);
  // Set hitSurfaces and distance
  List<scalarList> hitDistSqr{hitInfo.size()};
  FOR_ALL(hitInfo, pointI) {
    const List<pointIndexHit>& pHits = hitInfo[pointI];
    labelList& pSurfaces = hitSurfaces[pointI];
    pSurfaces.setSize(pHits.size());
    pSurfaces = 0;
    scalarList& pDistSqr = hitDistSqr[pointI];
    pDistSqr.setSize(pHits.size());
    FOR_ALL(pHits, i) {
      pDistSqr[i] = magSqr(pHits[i].hitPoint() - start[pointI]);
    }
  }
  if (surfacesToTest.size() > 1) {
    // Test the other surfaces and merge (according to distance from start).
    for (label testI = 1; testI < surfacesToTest.size(); testI++) {
      List<List<pointIndexHit>> surfHits;
      allSurfaces[surfacesToTest[testI]].findLineAll
      (
        start,
        end,
        surfHits
      );
      FOR_ALL(surfHits, pointI) {
        mergeHits
        (
          start[pointI],          // Current segment
          testI,                  // Surface and its hits
          surfHits[pointI],
          hitSurfaces[pointI],    // Merge into overall hit info
          hitInfo[pointI],
          hitDistSqr[pointI]
        );
      }
    }
  }
}


//Find intersections of edge nearest to both endpoints.
void mousse::searchableSurfacesQueries::findNearestIntersection
(
 const PtrList<searchableSurface>& allSurfaces,
 const labelList& surfacesToTest,
 const pointField& start,
 const pointField& end,
 labelList& surface1,
 List<pointIndexHit>& hit1,
 labelList& surface2,
 List<pointIndexHit>& hit2
)
{
 // 1. intersection from start to end
 // Initialize arguments
 surface1.setSize(start.size());
 surface1 = -1;
 hit1.setSize(start.size());
 // Current end of segment to test.
 pointField nearest{end};
 // Work array
 List<pointIndexHit> nearestInfo{start.size()};
 FOR_ALL(surfacesToTest, testI) {
   // See if any intersection between start and current nearest
   allSurfaces[surfacesToTest[testI]].findLine
   (
     start,
     nearest,
     nearestInfo
   );
   FOR_ALL(nearestInfo, pointI) {
     if (nearestInfo[pointI].hit()) {
       hit1[pointI] = nearestInfo[pointI];
       surface1[pointI] = testI;
       nearest[pointI] = hit1[pointI].hitPoint();
     }
   }
 }
 // 2. intersection from end to last intersection
 // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 // Find the nearest intersection from end to start. Note that we
 // initialize to the first intersection (if any).
 surface2 = surface1;
 hit2 = hit1;
 // Set current end of segment to test.
 FOR_ALL(nearest, pointI) {
   if (hit1[pointI].hit()) {
     nearest[pointI] = hit1[pointI].hitPoint();
   } else {
     // Disable testing by setting to end.
     nearest[pointI] = end[pointI];
   }
 }
 FOR_ALL(surfacesToTest, testI) {
   // See if any intersection between end and current nearest
   allSurfaces[surfacesToTest[testI]].findLine(end, nearest, nearestInfo);
   FOR_ALL(nearestInfo, pointI) {
     if (nearestInfo[pointI].hit()) {
       hit2[pointI] = nearestInfo[pointI];
       surface2[pointI] = testI;
       nearest[pointI] = hit2[pointI].hitPoint();
     }
   }
 }
}


// Find nearest. Return -1 or nearest point
void mousse::searchableSurfacesQueries::findNearest
(
  const PtrList<searchableSurface>& allSurfaces,
  const labelList& surfacesToTest,
  const pointField& samples,
  const scalarField& nearestDistSqr,
  labelList& nearestSurfaces,
  List<pointIndexHit>& nearestInfo
)
{
  // Initialise
  nearestSurfaces.setSize(samples.size());
  nearestSurfaces = -1;
  nearestInfo.setSize(samples.size());
  // Work arrays
  scalarField minDistSqr{nearestDistSqr};
  List<pointIndexHit> hitInfo{samples.size()};
  FOR_ALL(surfacesToTest, testI) {
    allSurfaces[surfacesToTest[testI]].findNearest
    (
      samples,
      minDistSqr,
      hitInfo
    );
    // Update minDistSqr and arguments
    FOR_ALL(hitInfo, pointI) {
      if (hitInfo[pointI].hit()) {
        minDistSqr[pointI] = magSqr(hitInfo[pointI].hitPoint()
                                    - samples[pointI]);
        nearestInfo[pointI] = hitInfo[pointI];
        nearestSurfaces[pointI] = testI;
      }
    }
  }
}


// Find nearest. Return -1 or nearest point
void mousse::searchableSurfacesQueries::findNearest
(
  const PtrList<searchableSurface>& allSurfaces,
  const labelList& surfacesToTest,
  const pointField& samples,
  const scalarField& nearestDistSqr,
  const labelList& regionIndices,
  labelList& nearestSurfaces,
  List<pointIndexHit>& nearestInfo
)
{
  if (regionIndices.empty()) {
    findNearest
    (
      allSurfaces,
      surfacesToTest,
      samples,
      nearestDistSqr,
      nearestSurfaces,
      nearestInfo
    );
  }
  // Initialise
  nearestSurfaces.setSize(samples.size());
  nearestSurfaces = -1;
  nearestInfo.setSize(samples.size());
  // Work arrays
  scalarField minDistSqr{nearestDistSqr};
  List<pointIndexHit> hitInfo{samples.size()};
  FOR_ALL(surfacesToTest, testI) {
    allSurfaces[surfacesToTest[testI]].findNearest
    (
      samples,
      minDistSqr,
      regionIndices,
      hitInfo
    );
    // Update minDistSqr and arguments
    FOR_ALL(hitInfo, pointI) {
      if (hitInfo[pointI].hit()) {
        minDistSqr[pointI] = magSqr(hitInfo[pointI].hitPoint()
                                    - samples[pointI]);
        nearestInfo[pointI] = hitInfo[pointI];
        nearestSurfaces[pointI] = testI;
      }
    }
  }
}


void mousse::searchableSurfacesQueries::signedDistance
(
  const PtrList<searchableSurface>& allSurfaces,
  const labelList& surfacesToTest,
  const pointField& samples,
  const scalarField& nearestDistSqr,
  const volumeType illegalHandling,
  labelList& nearestSurfaces,
  scalarField& distance
)
{
  // Initialise
  distance.setSize(samples.size());
  distance = -GREAT;
  // Find nearest
  List<pointIndexHit> nearestInfo;
  findNearest
  (
    allSurfaces,
    surfacesToTest,
    samples,
    nearestDistSqr,
    nearestSurfaces,
    nearestInfo
  );
  // Determine sign of nearest. Sort by surface to do this.
  DynamicField<point> surfPoints{samples.size()};
  DynamicList<label> surfIndices{samples.size()};
  FOR_ALL(surfacesToTest, testI) {
    // Extract samples on this surface
    surfPoints.clear();
    surfIndices.clear();
    FOR_ALL(nearestSurfaces, i) {
      if (nearestSurfaces[i] == testI) {
        surfPoints.append(samples[i]);
        surfIndices.append(i);
      }
    }
    // Calculate sideness of these surface points
    List<volumeType> volType;
    allSurfaces[surfacesToTest[testI]].getVolumeType(surfPoints, volType);
    // Push back to original
    FOR_ALL(volType, i) {
      label pointI = surfIndices[i];
      scalar dist = mag(samples[pointI] - nearestInfo[pointI].hitPoint());
      volumeType vT = volType[i];
      if (vT == volumeType::OUTSIDE) {
        distance[pointI] = dist;
      } else if (vT == volumeType::INSIDE) {
        distance[i] = -dist;
      } else {
        switch (illegalHandling)
        {
          case volumeType::OUTSIDE:
          {
            distance[pointI] = dist;
            break;
          }
          case volumeType::INSIDE:
          {
            distance[pointI] = -dist;
            break;
          }
          default:
          {
            FATAL_ERROR_IN("signedDistance()")
              << "getVolumeType failure,"
              << " neither INSIDE or OUTSIDE."
              << " point:" << surfPoints[i]
              << " surface:"
              << allSurfaces[surfacesToTest[testI]].name()
              << " volType:"
              << volumeType::names[vT]
              << exit(FatalError);
            break;
          }
        }
      }
    }
  }
}


mousse::boundBox mousse::searchableSurfacesQueries::bounds
(
  const PtrList<searchableSurface>& allSurfaces,
  const labelList& surfacesToTest
)
{
  pointField bbPoints{2*surfacesToTest.size()};
  FOR_ALL(surfacesToTest, testI) {
    const searchableSurface& surface(allSurfaces[surfacesToTest[testI]]);
    bbPoints[2*testI] = surface.bounds().min();
    bbPoints[2*testI + 1] = surface.bounds().max();
  }
  return {bbPoints};
}


//- Calculate point which is on a set of surfaces.
mousse::pointIndexHit mousse::searchableSurfacesQueries::facesIntersection
(
  const PtrList<searchableSurface>& allSurfaces,
  const labelList& surfacesToTest,
  const scalar initDistSqr,
  const scalar convergenceDistSqr,
  const point& start
)
{
  // Get four starting points. Take these as the projection of the
  // starting point onto the surfaces and the mid point
  List<point> nearest{surfacesToTest.size()+1};
  point sumNearest = vector::zero;
  FOR_ALL(surfacesToTest, i) {
    pointIndexHit hit
    {
      tempFindNearest(allSurfaces[surfacesToTest[i]], start, initDistSqr)
    };
    if (hit.hit()) {
      nearest[i] = hit.hitPoint();
      sumNearest += nearest[i];
    } else {
      FATAL_ERROR_IN
      (
        "searchableSurfacesQueries::facesIntersection"
        "(const labelList&, const scalar, const scalar, const point&)"
      )
      << "Did not find point within distance "
      << initDistSqr << " of starting point " << start
      << " on surface "
      << allSurfaces[surfacesToTest[i]].IOobject::name()
      << abort(FatalError);
    }
  }
  nearest.last() = sumNearest / surfacesToTest.size();
  // Get the sum of distances (initial evaluation)
  List<scalar> nearestDist{nearest.size()};
  FOR_ALL(nearestDist, i) {
    nearestDist[i] = sumDistSqr
    (
      allSurfaces,
      surfacesToTest,
      initDistSqr,
      nearest[i]
    );
  }
  //- Downhill Simplex method
  bool converged = morphTet
  (
    allSurfaces,
    surfacesToTest,
    initDistSqr,
    convergenceDistSqr,
    2000,
    nearest,
    nearestDist
  );
  pointIndexHit intersection;
  if (converged) {
    // Project nearest onto 0th surface.
    intersection = tempFindNearest
    (
      allSurfaces[surfacesToTest[0]],
      nearest[0],
      nearestDist[0]
    );
  }
  return intersection;
}
