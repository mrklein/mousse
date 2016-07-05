// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
#include "iomanip.hpp"


// Member Functions 
void mousse::triSurface::writeAC(Ostream& os) const
{
  // Write with patches as separate objects under "world" object.
  // Header is taken over from sample file.
  // Defines separate materials for all patches. Recycle colours.
  // Define 8 standard colours as r,g,b components
  static scalar colourMap[] =
  {
    1, 1, 1,
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
    1, 1, 0,
    0, 1, 1,
    1, 0, 1,
    0.5, 0.5, 1
  };
  // Calculate patch face indexing
  labelList faceMap;
  surfacePatchList myPatches{calcPatches(faceMap)};
  // Write header. Define materials.
  os << "AC3Db" << endl;
  FOR_ALL(myPatches, patchI) {
    const word& pName = myPatches[patchI].name();
    label colourI = patchI % 8;
    label colourCompI = 3 * colourI;
    os << "MATERIAL \"" << pName << "Mat\" rgb "
      << colourMap[colourCompI] << ' ' << colourMap[colourCompI+1]
      << ' ' << colourMap[colourCompI+2]
      << "  amb 0.2 0.2 0.2  emis 0 0 0  spec 0.5 0.5 0.5  shi 10"
      << "  trans 0"
      << endl;
  }
  os << "OBJECT world" << endl
    << "kids " << myPatches.size() << endl;
  // Write patch points & faces.
  label faceIndex = 0;
  FOR_ALL(myPatches, patchI) {
    const surfacePatch& sp = myPatches[patchI];
    os << "OBJECT poly" << endl << "name \"" << sp.name() << '"' << endl;
    // Create patch with only patch faces included for ease of addressing
    boolList include{size(), false};
    FOR_ALL(sp, patchFaceI) {
      const label faceI = faceMap[faceIndex++];
      include[faceI] = true;
    }
    labelList pointMap;
    labelList faceMap;
    triSurface patch = subsetMesh(include, pointMap, faceMap);
    // Now we have triSurface for this patch alone. Write it.
    os << "numvert " << patch.nPoints() << endl;
    FOR_ALL(patch.localPoints(), ptI) {
      const point& pt = patch.localPoints()[ptI];
      os << pt.x() << ' ' << pt.y() << ' ' << pt.z() << endl;
    }
    os << "numsurf " << patch.localFaces().size() << endl;
    FOR_ALL(patch.localFaces(), faceI) {
      const labelledTri& f = patch.localFaces()[faceI];
      os << "SURF 0x20" << endl          // polygon
        << "mat " << patchI << endl
        << "refs " << f.size() << endl;
      os << f[0] << " 0 0" << endl;
      os << f[1] << " 0 0" << endl;
      os << f[2] << " 0 0" << endl;
    }
    os << "kids 0" << endl;
  }
}
