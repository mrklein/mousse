// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
namespace mousse
{
// Member Functions 
void triSurface::writeOFF(const bool writeSorted, Ostream& os) const
{
  // Write header
  os  << "OFF" << endl
    << "# Geomview OFF file" << endl
    << "# Regions:" << endl;
  labelList faceMap;
  surfacePatchList myPatches(calcPatches(faceMap));
  // Print patch names as comment
  forAll(myPatches, patchI)
  {
    os  << "#     " << patchI << "    "
      << myPatches[patchI].name() << endl;
  }
  os  << nl << endl;
  const pointField& ps = points();
  os  << "# nPoints  nTriangles  nEdges" << endl
    << ps.size()
    << ' ' << size()
    << ' ' << nEdges()
    << nl << endl;
  // Write vertex coords
  forAll(ps, pointi)
  {
    os  << ps[pointi].x() << ' '
      << ps[pointi].y() << ' '
      << ps[pointi].z() << " #" << pointi << endl;
  }
  os  << endl;
  if (writeSorted)
  {
    label faceIndex = 0;
    forAll(myPatches, patchI)
    {
      // Print all faces belonging to this patch
      for
      (
        label patchFaceI = 0;
        patchFaceI < myPatches[patchI].size();
        patchFaceI++
      )
      {
        const label faceI = faceMap[faceIndex++];
        os  << "3 "
          << operator[](faceI)[0] << ' '
          << operator[](faceI)[1] << ' '
          << operator[](faceI)[2] << ' '
          << operator[](faceI).region()
          << endl;
      }
    }
  }
  else
  {
    forAll(*this, faceI)
    {
      os  << "3 "
        << operator[](faceI)[0] << ' '
        << operator[](faceI)[1] << ' '
        << operator[](faceI)[2] << ' '
        << operator[](faceI).region()
        << endl;
    }
  }
}
}  // namespace mousse
