// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wrl_surface_format.hpp"
#include "ostream.hpp"
#include "ofstream.hpp"
#include "list_ops.hpp"


// Constructors 
template<class Face>
mousse::fileFormats::WRLsurfaceFormat<Face>::WRLsurfaceFormat()
{}


// Member Functions 
template<class Face>
void mousse::fileFormats::WRLsurfaceFormat<Face>::write
(
  const fileName& filename,
  const MeshedSurfaceProxy<Face>& surf
)
{
  const pointField& pointLst = surf.points();
  const List<Face>& faceLst = surf.faces();
  const List<label>& faceMap = surf.faceMap();
  // for no zones, suppress the group name
  const List<surfZone>& zones =
  (
    surf.surfZones().empty()
    ? surfaceFormatsCore::oneZone(faceLst, "")
    : surf.surfZones()
  );
  const bool useFaceMap = (surf.useFaceMap() && zones.size() > 1);
  OFstream os{filename};
  if (!os.good()) {
    FATAL_ERROR_IN
    (
      "fileFormats::WRLsurfaceFormat::write"
      "(const fileName&, const MeshedSurfaceProxy<Face>&)"
    )
    << "Cannot open file for writing " << filename
    << exit(FatalError);
  }
  writeHeader(os, pointLst, faceLst.size(), zones);
  os << "\n"
    "Group {\n"
    " children [\n"
    "  Shape {\n";
  writeAppearance(os);
  os <<
    "   geometry IndexedFaceSet {\n"
    "    coord Coordinate {\n"
    "     point [\n";
  // Write vertex coords
  FOR_ALL(pointLst, ptI) {
    const point& pt = pointLst[ptI];
    os << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
  }
  os<<
    "     ]\n"                     // end point
    "    }\n"                      // end coord Coordinate
    "    coordIndex [\n";
  label faceIndex = 0;
  FOR_ALL(zones, zoneI) {
    const surfZone& zone = zones[zoneI];
    if (useFaceMap) {
      FOR_ALL(zone, localFaceI) {
        const Face& f = faceLst[faceMap[faceIndex++]];
        FOR_ALL(f, fp) {
          os << f[fp] << ' ';
        }
        os << "-1,\n";
      }
    } else {
      FOR_ALL(zone, localFaceI) {
        const Face& f = faceLst[faceIndex++];
        FOR_ALL(f, fp) {
          os << ' ' << f[fp];
        }
        os << " -1,\n";
      }
    }
  }
  os <<
    "    ]\n"                      // end coordIndex
    "   }\n"                       // end geometry IndexedFaceSet
    "  }\n"                        // end Shape
    " ]\n"                         // end children
    "}\n";                         // end Group
}

