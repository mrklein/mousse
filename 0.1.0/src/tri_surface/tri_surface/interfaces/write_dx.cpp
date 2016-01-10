// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
namespace mousse
{
// Member Functions 
// Geometry (positions + connections)
// writeSorted: sort acc. to patch
void triSurface::writeDXGeometry
(
  const bool writeSorted,
  Ostream& os
) const
{
  labelList faceMap;
  surfacePatchList myPatches(calcPatches(faceMap));
  // Print patch names as comment
  os<< "# Patches:" << endl;
  FOR_ALL(myPatches, patchI)
  {
    os  << "#     " << patchI << "    "
      << myPatches[patchI].name() << endl;
  }
  os<< nl << endl;
  // Write vertex coordinates
  os<< "# The irregular positions" << endl
    << "object 1 class array type float rank 1 shape 3 items "
    << nPoints() << " data follows" << endl;
  FOR_ALL(localPoints(), pointI)
  {
    const point& pt = localPoints()[pointI];
    os  << pt.x() << ' ' << pt.y() << ' ' << pt.z() << endl;
  }
  os  << endl;
  os  << "# The irregular connections (triangles)" << endl
    << "object 2 class array type int rank 1 shape 3 items "
    << size() << " data follows" << endl;
  if (writeSorted)
  {
    label faceIndex = 0;
    FOR_ALL(myPatches, patchI)
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
        const labelledTri& f = localFaces()[faceI];
        os  << f[0] << ' ' << f[1] << ' ' << f[2] << endl;
      }
    }
  }
  else
  {
    FOR_ALL(*this, faceI)
    {
      const labelledTri& f = localFaces()[faceI];
      os  << f[0] << ' ' << f[1] << ' ' << f[2] << endl;
    }
  }
  os<< "attribute \"element type\" string \"triangles\"" << endl
    << "attribute \"ref\" string \"positions\"" << endl << endl;
}
// Standard trailer
void triSurface::writeDXTrailer(Ostream& os) const
{
  os<< "# the field, with three components: \"positions\", \"connections\""
    << ", and \"data\"" << endl
    << "object \"irregular positions irregular connections\" class field"
    << endl
    << "component \"positions\" value 1" << endl
    << "component \"connections\" value 2" << endl
    << "component \"data\" value 3" << endl;
}
// Geometry only (data field is either faceIndex or patchIndex)
void triSurface::writeDX(const bool writeSorted, Ostream& os) const
{
  writeDXGeometry(writeSorted, os);
  os<< "object 3 class array type float rank 0 items " << size()
    << " data follows" << endl;
  if (writeSorted)
  {
    // Write patch number as data
    labelList faceMap;
    surfacePatchList myPatches(calcPatches(faceMap));
    FOR_ALL(myPatches, patchI)
    {
      FOR_ALL(myPatches[patchI], patchFaceI)
      {
        os  << patchI << endl;
      }
    }
  }
  else
  {
    // Write face number as data
    FOR_ALL(*this, faceI)
    {
      os  << faceI << endl;
    }
  }
  os<< endl << "attribute \"dep\" string \"connections\"" << endl << endl;
  writeDXTrailer(os);
  os<< "end" << endl;
}
// Geometry + scalar data
void triSurface::writeDX(const scalarField& field, Ostream& os) const
{
  writeDXGeometry(false, os);
  if (field.size() == size())
  {
    // Connections dependent data
    os<< "object 3 class array type float rank 0 items " << field.size()
      << " data follows" << endl;
    FOR_ALL(field, faceI)
    {
      os<< field[faceI] << endl;
    }
    os<< endl
      << "attribute \"dep\" string \"connections\"" << endl << endl;
  }
  else if (field.size() == nPoints())
  {
    // Positions dependent data
    os<< "object 3 class array type float rank 0 items " << field.size()
      << " data follows" << endl;
    FOR_ALL(field, pointI)
    {
      os  << field[pointI] << endl;
    }
    os  << endl
      << "attribute \"dep\" string \"positions\"" << endl << endl;
  }
  else
  {
    FATAL_ERROR_IN
    (
      "writeDX(const scalarField&, Ostream&)"
    )
    << "Illegal field size " << field.size() << " is not equal "
    << " to number of faces " << size() << " or to number "
    << " of points " << nPoints() << exit(FatalError);
  }
  writeDXTrailer(os);
  os<< "end" << endl;
}
// Geometry + vector data
void triSurface::writeDX(const vectorField& field, Ostream& os) const
{
  writeDXGeometry(false, os);
  if (field.size() == size())
  {
    // Connections dependent data
    os<< "object 3 class array type float rank 1 shape 3 items "
      << field.size() << " data follows" << endl;
    FOR_ALL(field, faceI)
    {
      os<< field[faceI].x() << ' '
        << field[faceI].y() << ' '
        << field[faceI].z() << endl;
    }
    os<< endl
      << "attribute \"dep\" string \"connections\"" << endl << endl;
  }
  else if (field.size() == nPoints())
  {
    // Positions dependent data
    os<< "object 3 class array type float rank 1 shape 3 items "
      << field.size() << " data follows" << endl;
    FOR_ALL(field, pointI)
    {
      os  << field[pointI].x() << ' '
        << field[pointI].y() << ' '
        << field[pointI].z() << endl;
    }
    os  << endl
      << "attribute \"dep\" string \"positions\"" << endl << endl;
  }
  else
  {
    FATAL_ERROR_IN
    (
      "writeDX(const vectorField&, Ostream&)"
    )   << "Illegal field size " << field.size() << " is not equal "
      << " to number of faces " << size() << " or to number "
      << " of points " << nPoints() << exit(FatalError);
  }
  writeDXTrailer(os);
  os  << "end" << endl;
}
}  // namespace mousse
