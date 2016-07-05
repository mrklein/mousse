// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "stl_surface_format.hpp"
#include "list_ops.hpp"
#include "tri_point_ref.hpp"


// Private Member Functions 
template<class Face>
inline void mousse::fileFormats::STLsurfaceFormat<Face>::writeShell
(
  Ostream& os,
  const pointField& pointLst,
  const Face& f
)
{
  // calculate the normal ourselves, for flexibility and speed
  vector norm = triPointRef
  (
    pointLst[f[0]],
    pointLst[f[1]],
    pointLst[f[2]]
  ).normal();
  norm /= mag(norm) + VSMALL;
  // simple triangulation about f[0].
  // better triangulation should have been done before
  const point& p0 = pointLst[f[0]];
  for (label fp1 = 1; fp1 < f.size() - 1; ++fp1) {
    label fp2 = f.fcIndex(fp1);
    const point& p1 = pointLst[f[fp1]];
    const point& p2 = pointLst[f[fp2]];
    // write STL triangle
    os << " facet normal "
      << norm.x() << ' ' << norm.y() << ' ' << norm.z() << nl
      << "  outer loop\n"
      << "   vertex " << p0.x() << ' ' << p0.y() << ' ' << p0.z() << nl
      << "   vertex " << p1.x() << ' ' << p1.y() << ' ' << p1.z() << nl
      << "   vertex " << p2.x() << ' ' << p2.y() << ' ' << p2.z() << nl
      << "  endloop\n"
      << " endfacet" << endl;
  }
}


template<class Face>
inline void mousse::fileFormats::STLsurfaceFormat<Face>::writeShell
(
  ostream& os,
  const pointField& pointLst,
  const Face& f,
  const label zoneI
)
{
  // calculate the normal ourselves, for flexibility and speed
  vector norm = triPointRef
  {
    pointLst[f[0]],
    pointLst[f[1]],
    pointLst[f[2]]
  }.normal();
  norm /= mag(norm) + VSMALL;
  // simple triangulation about f[0].
  // better triangulation should have been done before
  const point& p0 = pointLst[f[0]];
  for (label fp1 = 1; fp1 < f.size() - 1; ++fp1) {
    label fp2 = f.fcIndex(fp1);
    STLtriangle stlTri
    {
      norm,
      p0,
      pointLst[f[fp1]],
      pointLst[f[fp2]],
      static_cast<unsigned short>(zoneI)
    };
    stlTri.write(os);
  }
}


// Constructors 
template<class Face>
mousse::fileFormats::STLsurfaceFormat<Face>::STLsurfaceFormat
(
  const fileName& filename
)
{
  read(filename);
}


// Member Functions 
template<class Face>
bool mousse::fileFormats::STLsurfaceFormat<Face>::read
(
  const fileName& filename
)
{
  this->clear();
  // read in the values
  STLsurfaceFormatCore reader{filename};
  // transfer points
  this->storedPoints().transfer(reader.points());
  // retrieve the original zone information
  List<word> names{reader.names().xfer()};
  List<label> sizes{reader.sizes().xfer()};
  List<label> zoneIds{reader.zoneIds().xfer()};
  // generate the (sorted) faces
  List<Face> faceLst{zoneIds.size()};
  if (reader.sorted()) {
    // already sorted - generate directly
    FOR_ALL(faceLst, faceI) {
      const label startPt = 3*faceI;
      faceLst[faceI] = triFace(startPt, startPt+1, startPt+2);
    }
  } else {
    // unsorted - determine the sorted order:
    // avoid SortableList since we discard the main list anyhow
    List<label> faceMap;
    sortedOrder(zoneIds, faceMap);
    // generate sorted faces
    FOR_ALL(faceMap, faceI) {
      const label startPt = 3*faceMap[faceI];
      faceLst[faceI] = triFace(startPt, startPt+1, startPt+2);
    }
  }
  zoneIds.clear();
  // transfer:
  this->storedFaces().transfer(faceLst);
  if (names.size()) {
    this->addZones(sizes, names);
  } else {
    this->addZones(sizes);
  }
  this->stitchFaces(SMALL);
  return true;
}


template<class Face>
void mousse::fileFormats::STLsurfaceFormat<Face>::writeAscii
(
  const fileName& filename,
  const MeshedSurfaceProxy<Face>& surf
)
{
  OFstream os{filename};
  if (!os.good()) {
    FATAL_ERROR_IN
    (
      "fileFormats::STLsurfaceFormat::writeAscii"
      "(const fileName&, const MeshedSurfaceProxy<Face>&)"
    )
    << "Cannot open file for writing " << filename
    << exit(FatalError);
  }
  const pointField& pointLst = surf.points();
  const List<Face>&  faceLst = surf.faces();
  const List<label>& faceMap = surf.faceMap();
  const List<surfZone>& zones =
  (
    surf.surfZones().empty()
    ? surfaceFormatsCore::oneZone(faceLst)
    : surf.surfZones()
  );
  const bool useFaceMap = (surf.useFaceMap() && zones.size() > 1);
  label faceIndex = 0;
  FOR_ALL(zones, zoneI) {
    // Print all faces belonging to this zone
    const surfZone& zone = zones[zoneI];
    os << "solid " << zone.name() << nl;
    if (useFaceMap) {
      FOR_ALL(zone, localFaceI) {
        const label faceI = faceMap[faceIndex++];
        writeShell(os, pointLst, faceLst[faceI]);
      }
    } else {
      FOR_ALL(zone, localFaceI) {
        writeShell(os, pointLst, faceLst[faceIndex++]);
      }
    }
    os << "endsolid " << zone.name() << endl;
  }
}


template<class Face>
void mousse::fileFormats::STLsurfaceFormat<Face>::writeBinary
(
  const fileName& filename,
  const MeshedSurfaceProxy<Face>& surf
)
{
  std::ofstream os{filename.c_str(), std::ios::binary};
  if (!os.good()) {
    FATAL_ERROR_IN
    (
      "fileFormats::STLsurfaceFormat::writeBinary"
      "(const fileName&, const MeshedSurfaceProxy<Face>&)"
    )
    << "Cannot open file for writing " << filename
    << exit(FatalError);
  }
  const pointField& pointLst = surf.points();
  const List<Face>& faceLst = surf.faces();
  const List<label>& faceMap = surf.faceMap();
  const List<surfZone>& zones =
  (
    surf.surfZones().size() > 1
    ? surf.surfZones()
    : surfaceFormatsCore::oneZone(faceLst)
  );
  const bool useFaceMap = (surf.useFaceMap() && zones.size() > 1);
  unsigned int nTris = 0;
  if (MeshedSurface<Face>::isTri()) {
    nTris = faceLst.size();
  } else {
    // count triangles for on-the-fly triangulation
    FOR_ALL(faceLst, faceI) {
      nTris += faceLst[faceI].size() - 2;
    }
  }
  // Write the STL header
  STLsurfaceFormatCore::writeHeaderBINARY(os, nTris);
  label faceIndex = 0;
  FOR_ALL(zones, zoneI) {
    const surfZone& zone = zones[zoneI];
    if (useFaceMap) {
      FOR_ALL(zone, localFaceI) {
        writeShell
        (
          os,
          pointLst,
          faceLst[faceMap[faceIndex++]],
          zoneI
        );
      }
    } else {
      FOR_ALL(zone, localFaceI) {
        writeShell
        (
          os,
          pointLst,
          faceLst[faceIndex++],
          zoneI
        );
      }
    }
  }
}


template<class Face>
void mousse::fileFormats::STLsurfaceFormat<Face>::writeAscii
(
  const fileName& filename,
  const UnsortedMeshedSurface<Face>& surf
)
{
  OFstream os{filename};
  if (!os.good()) {
    FATAL_ERROR_IN
    (
      "fileFormats::STLsurfaceFormat::writeAscii"
      "(const fileName&, const UnsortedMeshedSurface<Face>&)"
    )
    << "Cannot open file for writing " << filename
    << exit(FatalError);
  }
  // a single zone - we can skip sorting
  if (surf.zoneToc().size() == 1) {
    const pointField& pointLst = surf.points();
    const List<Face>& faceLst  = surf.faces();
    os << "solid " << surf.zoneToc()[0].name() << endl;
    FOR_ALL(faceLst, faceI) {
      writeShell(os, pointLst, faceLst[faceI]);
    }
    os << "endsolid " << surf.zoneToc()[0].name() << endl;
  } else {
    labelList faceMap;
    List<surfZone> zoneLst = surf.sortedZones(faceMap);
    writeAscii
    (
      filename,
      MeshedSurfaceProxy<Face>
      {
        surf.points(),
        surf.faces(),
        zoneLst,
        faceMap
      }
    );
  }
}


template<class Face>
void mousse::fileFormats::STLsurfaceFormat<Face>::writeBinary
(
  const fileName& filename,
  const UnsortedMeshedSurface<Face>& surf
)
{
  std::ofstream os(filename.c_str(), std::ios::binary);
  if (!os.good()) {
    FATAL_ERROR_IN
    (
      "fileFormats::STLsurfaceFormat::writeBinary"
      "(const fileName&, const UnsortedMeshedSurface<Face>&)"
    )
    << "Cannot open file for writing " << filename
    << exit(FatalError);
  }
  const pointField& pointLst = surf.points();
  const List<Face>& faceLst  = surf.faces();
  const List<label>& zoneIds  = surf.zoneIds();
  unsigned int nTris = 0;
  if (MeshedSurface<Face>::isTri()) {
    nTris = faceLst.size();
  } else {
    // count triangles for on-the-fly triangulation
    FOR_ALL(faceLst, faceI) {
      nTris += faceLst[faceI].size() - 2;
    }
  }
  // Write the STL header
  STLsurfaceFormatCore::writeHeaderBINARY(os, nTris);
  // always write unsorted
  FOR_ALL(faceLst, faceI) {
    writeShell
    (
      os,
      pointLst,
      faceLst[faceI],
      zoneIds[faceI]
    );
  }
}


template<class Face>
void mousse::fileFormats::STLsurfaceFormat<Face>::write
(
  const fileName& filename,
  const MeshedSurfaceProxy<Face>& surf
)
{
  const word ext = filename.ext();
  // handle 'stlb' as binary directly
  if (ext == "stlb") {
    writeBinary(filename, surf);
  } else {
    writeAscii(filename, surf);
  }
}


template<class Face>
void mousse::fileFormats::STLsurfaceFormat<Face>::write
(
  const fileName& filename,
  const UnsortedMeshedSurface<Face>& surf
)
{
  word ext = filename.ext();
  // handle 'stlb' as binary directly
  if (ext == "stlb") {
    writeBinary(filename, surf);
  } else {
    writeAscii(filename, surf);
  }
}
