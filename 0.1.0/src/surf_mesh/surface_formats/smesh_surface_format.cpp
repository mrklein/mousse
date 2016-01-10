// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "smesh_surface_format.hpp"

#include "clock.hpp"
#include "ifstream.hpp"
#include "ofstream.hpp"
#include "ostream.hpp"

// Constructors 
template<class Face>
mousse::fileFormats::SMESHsurfaceFormat<Face>::SMESHsurfaceFormat()
{}


// Member Functions 
template<class Face>
void mousse::fileFormats::SMESHsurfaceFormat<Face>::write
(
  const fileName& filename,
  const MeshedSurfaceProxy<Face>& surf
)
{
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
  OFstream os{filename};
  if (!os.good())
  {
    FATAL_ERROR_IN
    (
      "fileFormats::SMESHsurfaceFormat::write"
      "(const fileName&, const MeshedSurfaceProxy<Face>&)"
    )
    << "Cannot open file for writing " << filename
    << exit(FatalError);
  }
  // Write header
  os<< "# tetgen .smesh file written " << clock::dateTime().c_str() << nl
    << "# <points count=\"" << pointLst.size() << "\">" << nl
    << pointLst.size() << " 3" << nl;    // 3: dimensions
  // Write vertex coords
  FOR_ALL(pointLst, ptI)
  {
    const point& pt = pointLst[ptI];
    os << ptI << ' ' << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
  }
  os<< "# </points>" << nl
    << nl
    << "# <faces count=\"" << faceLst.size() << "\">" << endl;
  os << faceLst.size() << " 1" << endl;   // one attribute: zone number
  label faceIndex = 0;
  FOR_ALL(zones, zoneI)
  {
    const surfZone& zone = zones[zoneI];
    if (useFaceMap)
    {
      FOR_ALL(zone, localFaceI)
      {
        const Face& f = faceLst[faceMap[faceIndex++]];
        os << f.size();
        FOR_ALL(f, fp)
        {
          os << ' ' << f[fp];
        }
        os << ' ' << zoneI << endl;
      }
    }
    else
    {
      FOR_ALL(zones[zoneI], localFaceI)
      {
        const Face& f = faceLst[faceIndex++];
        os << f.size();
        FOR_ALL(f, fp)
        {
          os << ' ' << f[fp];
        }
        os << ' ' << zoneI << endl;
      }
    }
  }
  // write tail
  os<< "# </faces>" << nl
    << nl
    << "# no holes or regions:" << nl
    << '0' << nl        // holes
    << '0' << endl;     // regions
}
