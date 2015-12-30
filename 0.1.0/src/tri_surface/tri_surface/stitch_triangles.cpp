// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
#include "merge_points.hpp"
#include "packed_bool_list.hpp"
// Member Functions 
bool mousse::triSurface::stitchTriangles
(
  const scalar tol,
  bool verbose
)
{
  pointField& ps = storedPoints();
  // Merge points
  labelList pointMap;
  pointField newPoints;
  bool hasMerged = mergePoints(ps, tol, verbose, pointMap, newPoints);
  if (hasMerged)
  {
    if (verbose)
    {
      Pout<< "stitchTriangles : Merged from " << ps.size()
        << " points down to " << newPoints.size() << endl;
    }
    // Set the coordinates to the merged ones
    ps.transfer(newPoints);
    // Reset the triangle point labels to the unique points array
    label newTriangleI = 0;
    forAll(*this, i)
    {
      const labelledTri& tri = operator[](i);
      labelledTri newTri
      (
        pointMap[tri[0]],
        pointMap[tri[1]],
        pointMap[tri[2]],
        tri.region()
      );
      if
      (
        (newTri[0] != newTri[1])
      && (newTri[0] != newTri[2])
      && (newTri[1] != newTri[2])
      )
      {
        operator[](newTriangleI++) = newTri;
      }
      else if (verbose)
      {
        Pout<< "stitchTriangles : "
          << "Removing triangle " << i
          << " with non-unique vertices." << endl
          << "    vertices   :" << newTri << endl
          << "    coordinates:" << newTri.points(ps)
          << endl;
      }
    }
    if (newTriangleI != size())
    {
      if (verbose)
      {
        Pout<< "stitchTriangles : "
          << "Removed " << size() - newTriangleI
          << " triangles" << endl;
      }
      setSize(newTriangleI);
      // And possibly compact out any unused points (since used only
      // by triangles that have just been deleted)
      // Done in two passes to save memory (pointField)
      // 1. Detect only
      PackedBoolList pointIsUsed(ps.size());
      label nPoints = 0;
      forAll(*this, i)
      {
        const triSurface::FaceType& f = operator[](i);
        forAll(f, fp)
        {
          label pointI = f[fp];
          if (pointIsUsed.set(pointI, 1))
          {
            nPoints++;
          }
        }
      }
      if (nPoints != ps.size())
      {
        // 2. Compact.
        pointMap.setSize(ps.size());
        label newPointI = 0;
        forAll(pointIsUsed, pointI)
        {
          if (pointIsUsed[pointI])
          {
            ps[newPointI] = ps[pointI];
            pointMap[pointI] = newPointI++;
          }
        }
        ps.setSize(newPointI);
        newTriangleI = 0;
        forAll(*this, i)
        {
          const labelledTri& tri = operator[](i);
          operator[](newTriangleI++) = labelledTri
          (
            pointMap[tri[0]],
            pointMap[tri[1]],
            pointMap[tri[2]],
            tri.region()
          );
        }
      }
    }
  }
  return hasMerged;
}
