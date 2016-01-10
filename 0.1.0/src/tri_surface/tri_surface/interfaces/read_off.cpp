// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
#include "ifstream.hpp"
#include "istring_stream.hpp"
// Member Functions 
bool mousse::triSurface::readOFF(const fileName& OFFfileName)
{
  IFstream OFFfile(OFFfileName);
  if (!OFFfile.good())
  {
    FATAL_ERROR_IN("triSurface::readOFF(const fileName&)")
      << "Cannot read file " << OFFfileName
      << exit(FatalError);
  }
  // Read header
  string hdr = getLineNoComment(OFFfile);
  if (hdr != "OFF")
  {
    FATAL_ERROR_IN("triSurface::readOFF(const fileName&)")
      << "OFF file " << OFFfileName
      << " does not start with 'OFF'"
      << exit(FatalError);
  }
  label nPoints, nEdges, nElems;
  string line = getLineNoComment(OFFfile);
  {
    IStringStream lineStream(line);
    lineStream >> nPoints >> nElems >> nEdges;
  }
  // Read points
  pointField points(nPoints);
  FOR_ALL(points, pointi)
  {
    scalar x, y, z;
    line = getLineNoComment(OFFfile);
    {
      IStringStream lineStream(line);
      lineStream >> x >> y >> z;
    }
    points[pointi] = point(x, y, z);
  }
  // Read faces & triangulate them,
  DynamicList<labelledTri> tris(nElems);
  for (label faceI = 0; faceI < nElems; faceI++)
  {
    line = getLineNoComment(OFFfile);
    {
      IStringStream lineStream(line);
      label nVerts;
      lineStream >> nVerts;
      face f(nVerts);
      FOR_ALL(f, fp)
      {
        lineStream >> f[fp];
      }
      // Triangulate.
      if (nVerts == 3)
      {
        tris.append(labelledTri(f[0], f[1], f[2], 0));
      }
      else if (nVerts == 4)
      {
        tris.append(labelledTri(f[0], f[1], f[2], 0));
        tris.append(labelledTri(f[2], f[3], f[0], 0));
      }
      else
      {
        faceList triFaces(f.nTriangles(points));
        label nTri = 0;
        f.triangles(points, nTri, triFaces);
        FOR_ALL(triFaces, triFaceI)
        {
          const face& f = triFaces[triFaceI];
          tris.append(labelledTri(f[0], f[1], f[2], 0));
        }
      }
    }
  }
  tris.shrink();
  *this = triSurface(tris, points);
  return true;
}
