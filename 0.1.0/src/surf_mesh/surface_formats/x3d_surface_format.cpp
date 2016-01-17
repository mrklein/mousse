// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "x3d_surface_format.hpp"

#include "clock.hpp"
#include "ifstream.hpp"
#include "istring_stream.hpp"
#include "ostream.hpp"
#include "ofstream.hpp"
#include "list_ops.hpp"

// Constructors 
template<class Face>
mousse::fileFormats::X3DsurfaceFormat<Face>::X3DsurfaceFormat()
{}


// Member Functions 
template<class Face>
void mousse::fileFormats::X3DsurfaceFormat<Face>::write
(
  const fileName& filename,
  const MeshedSurfaceProxy<Face>& surf
)
{
  const pointField& pointLst = surf.points();
  const List<Face>&  faceLst = surf.faces();
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
  if (!os.good())
  {
    FATAL_ERROR_IN
    (
      "fileFormats::X3DsurfaceFormat::write"
      "(const fileName&, const MeshedSurfaceProxy<Face>&)"
    )
      << "Cannot open file for writing " << filename
      << exit(FatalError);
  }
  writeHeader(os);
  os  << "\n"
    "<Group>\n"
    " <Shape>\n";
  writeAppearance(os);
  // NOTE: we could provide an optimized IndexedTriangleSet output for
  // triangulated surfaces too
  os  <<
    "  <IndexedFaceSet coordIndex='\n";
  label faceIndex = 0;
  FOR_ALL(zones, zoneI)
  {
    const surfZone& zone = zones[zoneI];
    if (useFaceMap)
    {
      FOR_ALL(zone, localFaceI)
      {
        const Face& f = faceLst[faceMap[faceIndex++]];
        FOR_ALL(f, fp)
        {
          os << f[fp] << ' ';
        }
        os << "-1\n";
      }
    }
    else
    {
      FOR_ALL(zone, localFaceI)
      {
        const Face& f = faceLst[faceIndex++];
        FOR_ALL(f, fp)
        {
          os << f[fp] << ' ';
        }
        os << "-1\n";
      }
    }
  }
  os <<
    "' >\n"
    "    <Coordinate point='\n";
  // Write vertex coords
  FOR_ALL(pointLst, ptI)
  {
    const point& pt = pointLst[ptI];
    os << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
  }
  os <<
    "' />\n"                       // end Coordinate
    "   </IndexedFaceSet>\n"
    "  </Shape>\n"
    " </Group>\n"
    "</X3D>\n";
}
