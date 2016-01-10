// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "stl_surface_format_core.hpp"
#include "gzstream.h"
#include "os_specific.hpp"
#include "map.hpp"
#include "ifstream.hpp"
#include "ostream.hpp"
#undef DEBUG_STLBINARY
// Private Member Functions 
// check binary by getting the header and number of facets
// this seems to work better than the old token-based method
// - some programs (eg, pro-STAR) have 'solid' as the first word in
//   the binary header.
// - using wordToken can cause an abort if non-word (binary) content
//   is detected ... this is not exactly what we want.
int mousse::fileFormats::STLsurfaceFormatCore::detectBINARY
(
  const fileName& filename
)
{
  off_t dataFileSize = mousse::fileSize(filename);
  IFstream str(filename, IOstream::BINARY);
  istream& is = str().stdStream();
  // Read the STL header
  char header[headerSize];
  is.read(header, headerSize);
  // Check that stream is OK, if not this may be an ASCII file
  if (!is.good())
  {
    return 0;
  }
  // Read the number of triangles in the STl file
  // (note: read as int so we can check whether >2^31)
  int nTris;
  is.read(reinterpret_cast<char*>(&nTris), sizeof(unsigned int));
  // Check that stream is OK and number of triangles is positive,
  // if not this may be an ASCII file
  //
  // Also compare the file size with that expected from the number of tris
  // If the comparison is not sensible then it may be an ASCII file
  if
  (
    !is
  || nTris < 0
  || nTris < (dataFileSize - headerSize)/50
  || nTris > (dataFileSize - headerSize)/25
  )
  {
    return 0;
  }
  // looks like it might be BINARY, return number of triangles
  return nTris;
}
bool mousse::fileFormats::STLsurfaceFormatCore::readBINARY
(
  istream& is,
  const off_t dataFileSize
)
{
  sorted_ = true;
  // Read the STL header
  char header[headerSize];
  is.read(header, headerSize);
  // Check that stream is OK, if not this may be an ASCII file
  if (!is.good())
  {
    FATAL_ERROR_IN
    (
      "fileFormats::STLsurfaceFormatCore::readBINARY(IFstream&)"
    )
      << "problem reading header, perhaps file is not binary "
      << exit(FatalError);
  }
  // Read the number of triangles in the STl file
  // (note: read as int so we can check whether >2^31)
  int nTris;
  is.read(reinterpret_cast<char*>(&nTris), sizeof(unsigned int));
  // Check that stream is OK and number of triangles is positive,
  // if not this maybe an ASCII file
  //
  // Also compare the file size with that expected from the number of tris
  // If the comparison is not sensible then it may be an ASCII file
  if
  (
    !is
  || nTris < 0
  || nTris < int(dataFileSize - headerSize)/50
  || nTris > int(dataFileSize - headerSize)/25
  )
  {
    FATAL_ERROR_IN
    (
      "fileFormats::STLsurfaceFormatCore::readBINARY(istream&)"
    )
      << "problem reading number of triangles, perhaps file is not binary"
      << exit(FatalError);
  }
#ifdef DEBUG_STLBINARY
  Info<< "# " << nTris << " facets" << endl;
  label prevZone = -1;
#endif
  points_.setSize(3*nTris);
  zoneIds_.setSize(nTris);
  Map<label> lookup;
  DynamicList<label> dynSizes;
  label ptI = 0;
  label zoneI = -1;
  FOR_ALL(zoneIds_, faceI)
  {
    // Read an STL triangle
    STLtriangle stlTri(is);
    // transcribe the vertices of the STL triangle -> points
    points_[ptI++] = stlTri.a();
    points_[ptI++] = stlTri.b();
    points_[ptI++] = stlTri.c();
    // interprete stl attribute as a zone
    const label origId = stlTri.attrib();
    Map<label>::const_iterator fnd = lookup.find(origId);
    if (fnd != lookup.end())
    {
      if (zoneI != fnd())
      {
        // group appeared out of order
        sorted_ = false;
      }
      zoneI = fnd();
    }
    else
    {
      zoneI = dynSizes.size();
      lookup.insert(origId, zoneI);
      dynSizes.append(0);
    }
    zoneIds_[faceI] = zoneI;
    dynSizes[zoneI]++;
#ifdef DEBUG_STLBINARY
    if (prevZone != zoneI)
    {
      if (prevZone != -1)
      {
        Info<< "endsolid zone" << prevZone << nl;
      }
      prevZone = zoneI;
      Info<< "solid zone" << prevZone << nl;
    }
    Info<< " facet normal " << stlTri.normal() << nl
      << "  outer loop" << nl
      << "   vertex " << stlTri.a() << nl
      << "   vertex " << stlTri.b() << nl
      << "   vertex " << stlTri.c() << nl
      << "  outer loop" << nl
      << " endfacet" << endl;
#endif
  }
  names_.clear();
  sizes_.transfer(dynSizes);
  return true;
}
// Constructors 
mousse::fileFormats::STLsurfaceFormatCore::STLsurfaceFormatCore
(
  const fileName& filename
)
:
  sorted_(true),
  points_(0),
  zoneIds_(0),
  names_(0),
  sizes_(0)
{
  off_t dataFileSize = mousse::fileSize(filename);
  // auto-detect ascii/binary
  if (detectBINARY(filename))
  {
    readBINARY
    (
      IFstream(filename, IOstream::BINARY)().stdStream(),
      dataFileSize
    );
  }
  else
  {
    readASCII
    (
      IFstream(filename)().stdStream(),
      dataFileSize
    );
  }
}
// Destructor 
mousse::fileFormats::STLsurfaceFormatCore::~STLsurfaceFormatCore()
{}
// Member Functions 
void mousse::fileFormats::STLsurfaceFormatCore::writeHeaderBINARY
(
  ostream& os,
  unsigned int nTris
)
{
  // STL header with extra information about nTris
  char header[headerSize];
  sprintf(header, "STL binary file %u facets", nTris);
  // avoid trailing junk
  for (size_t i = strlen(header); i < headerSize; ++i)
  {
    header[i] = 0;
  }
  os.write(header, headerSize);
  os.write(reinterpret_cast<char*>(&nTris), sizeof(unsigned int));
}
