// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
#include "stl_triangle.hpp"
#include "primitive_patch.hpp"
#include "hash_table.hpp"
#include "hash_signed_label.hpp"
// Member Functions 
void mousse::triSurface::writeSTLASCII(const bool writeSorted, Ostream& os) const
{
  labelList faceMap;
  surfacePatchList myPatches(calcPatches(faceMap));
  if (writeSorted)
  {
    label faceIndex = 0;
    FOR_ALL(myPatches, patchI)
    {
      // Print all faces belonging to this region
      const surfacePatch& patch = myPatches[patchI];
      os  << "solid " << patch.name() << endl;
      for
      (
        label patchFaceI = 0;
        patchFaceI < patch.size();
        patchFaceI++
      )
      {
        const label faceI = faceMap[faceIndex++];
        const vector& n = faceNormals()[faceI];
        os  << "  facet normal "
          << n.x() << ' ' << n.y() << ' ' << n.z() << nl
          << "    outer loop" << endl;
        const labelledTri& f = (*this)[faceI];
        const point& pa = points()[f[0]];
        const point& pb = points()[f[1]];
        const point& pc = points()[f[2]];
        os  << "       vertex "
          << pa.x() << ' ' << pa.y() << ' ' << pa.z() << nl
          << "       vertex "
          << pb.x() << ' ' << pb.y() << ' ' << pb.z() << nl
          << "       vertex "
          << pc.x() << ' ' << pc.y() << ' ' << pc.z() << nl
          << "    endloop" << nl
          << "  endfacet" << endl;
      }
      os  << "endsolid " << patch.name() << endl;
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
    label currentPatchI = -1;
    FOR_ALL(*this, faceI)
    {
      if (currentPatchI != patchIDs[faceI])
      {
        if (currentPatchI != -1)
        {
          // Have already valid patch. Close it.
          os  << "endsolid " << myPatches[currentPatchI].name()
            << nl;
        }
        currentPatchI = patchIDs[faceI];
        os  << "solid " << myPatches[currentPatchI].name() << nl;
      }
      const vector& n = faceNormals()[faceI];
      os  << "  facet normal "
        << n.x() << ' ' << n.y() << ' ' << n.z() << nl
        << "    outer loop" << endl;
      const labelledTri& f = (*this)[faceI];
      const point& pa = points()[f[0]];
      const point& pb = points()[f[1]];
      const point& pc = points()[f[2]];
      os  << "       vertex "
        << pa.x() << ' ' << pa.y() << ' ' << pa.z() << nl
        << "       vertex "
        << pb.x() << ' ' << pb.y() << ' ' << pb.z() << nl
        << "       vertex "
        << pc.x() << ' ' << pc.y() << ' ' << pc.z() << nl
        << "    endloop" << nl
        << "  endfacet" << endl;
    }
    if (currentPatchI != -1)
    {
      os  << "endsolid " << myPatches[currentPatchI].name()
        << nl;
    }
  }
}
void mousse::triSurface::writeSTLBINARY(std::ostream& os) const
{
  // Write the STL header
  string header("Foam binary STL", STLheaderSize);
  os.write(header.c_str(), STLheaderSize);
  label nTris = size();
  os.write(reinterpret_cast<char*>(&nTris), sizeof(unsigned int));
  const vectorField& normals = faceNormals();
  FOR_ALL(*this, faceI)
  {
    const labelledTri& f = (*this)[faceI];
    // Convert vector into STL single precision
    STLpoint n(normals[faceI]);
    STLpoint pa(points()[f[0]]);
    STLpoint pb(points()[f[1]]);
    STLpoint pc(points()[f[2]]);
    STLtriangle stlTri(n, pa, pb, pc, f.region());
    stlTri.write(os);
  }
}
