// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
#include "stl_triangle.hpp"
#include "ifstream.hpp"
#include "os_specific.hpp"
#include "gzstream.h"
#include "float_vector.hpp"
#include "merge_points.hpp"


// Member Functions 
bool mousse::triSurface::readSTLBINARY(const fileName& STLfileName)
{
  bool compressed = false;
  autoPtr<istream> STLfilePtr
  {
    new ifstream{STLfileName.c_str(), std::ios::binary}
  };
  // If the file is compressed, decompress it before reading.
  if (!STLfilePtr->good() && isFile(STLfileName + ".gz", false)) {
    compressed = true;
    STLfilePtr.reset(new igzstream((STLfileName + ".gz").c_str()));
  }
  istream& STLfile = STLfilePtr();
  if (!STLfile.good()) {
    FATAL_ERROR_IN("triSurface::readSTLBINARY(const fileName&)")
      << "Cannot read file " << STLfileName
      << " or file " << STLfileName + ".gz"
      << exit(FatalError);
  }
  // Read the STL header
  char header[STLheaderSize];
  STLfile.read(header, STLheaderSize);
  // Check that stream is OK, if not this maybe an ASCII file
  if (!STLfile) {
    return false;
  }
  // Read the number of triangles in the STl file
  // (note: read as int so we can check whether >2^31)
  int nTris;
  STLfile.read(reinterpret_cast<char*>(&nTris), sizeof(unsigned int));
  // Check that stream is OK and number of triangles is positive,
  // if not this maybe an ASCII file
  if (!STLfile || nTris < 0) {
    return false;
  }
  // Compare the size of the file with that expected from the number of tris
  // If the comparison is not sensible then it maybe an ASCII file
  if (!compressed) {
    label dataFileSize = mousse::fileSize(STLfileName) - 80;
    if (nTris < dataFileSize/50 || nTris > dataFileSize/25) {
      return false;
    }
  }
  // Everything OK so go ahead and read the triangles.
  // Allocate storage for raw points
  List<floatVector> STLpoints{3*nTris};
  setSize(nTris);
  label pointI = 0;
  for (label i = 0; i < nTris; i++) {
    // Read an STL triangle
    STLtriangle stlTri{STLfile};
    // Set the STLpoints to the vertices of the STL triangle
    STLpoints[pointI++] = stlTri.a();
    STLpoints[pointI++] = stlTri.b();
    STLpoints[pointI++] = stlTri.c();
    operator[](i).region() = stlTri.attrib();
  }
  // Stitch points
  labelList pointMap;
  label nUniquePoints = mergePoints
  (
    STLpoints,
    10*SMALL,               // merge distance
    false,                  // verbose
    pointMap                // old to new
  );
  pointField& sp = storedPoints();
  sp.setSize(nUniquePoints);
  FOR_ALL(STLpoints, pointI) {
    const floatVector& pt = STLpoints[pointI];
    sp[pointMap[pointI]] =
      vector{scalar(pt.x()), scalar(pt.y()), scalar(pt.z())};
  }
  // Assign triangles
  pointI = 0;
  FOR_ALL(*this, i) {
    operator[](i)[0] = pointMap[pointI++];
    operator[](i)[1] = pointMap[pointI++];
    operator[](i)[2] = pointMap[pointI++];
  }
  return true;
}

