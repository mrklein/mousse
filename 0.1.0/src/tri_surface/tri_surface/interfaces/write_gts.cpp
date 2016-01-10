// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
namespace mousse
{
// Member Functions 
void triSurface::writeGTS(const bool writeSorted, Ostream& os) const
{
  // Write header
  os  << "# GTS file" << endl
    << "# Regions:" << endl;
  labelList faceMap;
  surfacePatchList myPatches(calcPatches(faceMap));
  // Print patch names as comment
  FOR_ALL(myPatches, patchI)
  {
    os  << "#     " << patchI << "    "
      << myPatches[patchI].name() << endl;
  }
  os  << "#" << endl;
  const pointField& ps = points();
  os  << "# nPoints  nEdges  nTriangles" << endl
    << ps.size() << ' ' << nEdges() << ' ' << size() << endl;
  // Write vertex coords
  FOR_ALL(ps, pointi)
  {
    os  << ps[pointi].x() << ' '
      << ps[pointi].y() << ' '
      << ps[pointi].z() << endl;
  }
  // Write edges.
  // Note: edges are in local point labels so convert
  const edgeList& es = edges();
  const labelList& meshPts = meshPoints();
  FOR_ALL(es, edgei)
  {
    os  << meshPts[es[edgei].start()] + 1 << ' '
      << meshPts[es[edgei].end()] + 1 << endl;
  }
  // Write faces in terms of edges.
  const labelListList& faceEs = faceEdges();
  if (writeSorted)
  {
    label faceIndex = 0;
    FOR_ALL(myPatches, patchI)
    {
      for
      (
        label patchFaceI = 0;
        patchFaceI < myPatches[patchI].size();
        patchFaceI++
      )
      {
        const label faceI = faceMap[faceIndex++];
        const labelList& fEdges = faceEdges()[faceI];
        os<< fEdges[0] + 1 << ' '
          << fEdges[1] + 1 << ' '
          << fEdges[2] + 1 << ' '
          << (*this)[faceI].region() << endl;
      }
    }
  }
  else
  {
    FOR_ALL(faceEs, faceI)
    {
      const labelList& fEdges = faceEdges()[faceI];
      os<< fEdges[0] + 1 << ' '
        << fEdges[1] + 1 << ' '
        << fEdges[2] + 1 << ' '
        << (*this)[faceI].region() << endl;
    }
  }
}
}  // namespace mousse
