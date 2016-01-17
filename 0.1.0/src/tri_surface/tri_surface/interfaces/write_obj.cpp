// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
namespace mousse
{
// Member Functions 
void triSurface::writeOBJ(const bool writeSorted, Ostream& os) const
{
  // Write header
  os<< "# Wavefront OBJ file" << nl
    << "# Regions:" << nl;
  labelList faceMap;
  surfacePatchList myPatches(calcPatches(faceMap));
  const pointField& ps = points();
  // Print patch names as comment
  FOR_ALL(myPatches, patchI)
  {
    os<< "#     " << patchI << "    "
      << myPatches[patchI].name() << nl;
  }
  os<< "#" << nl;
  os<< "# points    : " << ps.size() << nl
    << "# triangles : " << size() << nl
    << "#" << nl;
  // Write vertex coords
  FOR_ALL(ps, pointi)
  {
    os<< "v "
      << ps[pointi].x() << ' '
      << ps[pointi].y() << ' '
      << ps[pointi].z() << nl;
  }
  if (writeSorted)
  {
    label faceIndex = 0;
    FOR_ALL(myPatches, patchI)
    {
      // Print all faces belonging to this patch
      os<< "g " << myPatches[patchI].name() << nl;
      for
      (
        label patchFaceI = 0;
        patchFaceI < myPatches[patchI].size();
        patchFaceI++
      )
      {
        const label faceI = faceMap[faceIndex++];
        os<< "f "
          << operator[](faceI)[0] + 1 << ' '
          << operator[](faceI)[1] + 1 << ' '
          << operator[](faceI)[2] + 1
          //<< "  # " << operator[](faceI).region()
          << nl;
      }
    }
  }
  else
  {
    // Get patch (=compact region) per face
    labelList patchIDs(size());
    FOR_ALL(myPatches, patchI)
    {
      label faceI = myPatches[patchI].start();
      FOR_ALL(myPatches[patchI], i)
      {
        patchIDs[faceMap[faceI++]] = patchI;
      }
    }
    label prevPatchI = -1;
    FOR_ALL(*this, faceI)
    {
      if (prevPatchI != patchIDs[faceI])
      {
        prevPatchI = patchIDs[faceI];
        os  << "g " << myPatches[patchIDs[faceI]].name() << nl;
      }
      os<< "f "
        << operator[](faceI)[0] + 1 << ' '
        << operator[](faceI)[1] + 1 << ' '
        << operator[](faceI)[2] + 1
        << nl;
    }
  }
}
}  // namespace mousse
