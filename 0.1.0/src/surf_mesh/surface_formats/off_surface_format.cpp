// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "off_surface_format.hpp"

#include "clock.hpp"
#include "ifstream.hpp"
#include "istring_stream.hpp"
#include "ostream.hpp"
#include "ofstream.hpp"

// Constructors 
template<class Face>
mousse::fileFormats::OFFsurfaceFormat<Face>::OFFsurfaceFormat
(
  const fileName& filename
)
{
  read(filename);
}


// Member Functions 
template<class Face>
bool mousse::fileFormats::OFFsurfaceFormat<Face>::read
(
  const fileName& filename
)
{
  const bool mustTriangulate = this->isTri();
  this->clear();
  IFstream is{filename};
  if (!is.good())
  {
    FATAL_ERROR_IN
    (
      "fileFormats::OFFsurfaceFormat::read(const fileName&)"
    )
    << "Cannot read file " << filename
    << exit(FatalError);
  }
  // Read header
  string hdr = this->getLineNoComment(is);
  if (hdr != "OFF")
  {
    FATAL_ERROR_IN
    (
      "fileFormats::OFFsurfaceFormat::read(const fileName&)"
    )
    << "OFF file " << filename << " does not start with 'OFF'"
    << exit(FatalError);
  }
  // get dimensions
  label nPoints, nElems, nEdges;
  string line = this->getLineNoComment(is);
  {
    IStringStream lineStream(line);
    lineStream >> nPoints >> nElems >> nEdges;
  }
  // Read points
  pointField pointLst(nPoints);
  FOR_ALL(pointLst, pointI)
  {
    scalar x, y, z;
    line = this->getLineNoComment(is);
    {
      IStringStream lineStream(line);
      lineStream >> x >> y >> z;
    }
    pointLst[pointI] = point(x, y, z);
  }
  // Read faces - ignore optional zone information
  // use a DynamicList for possible on-the-fly triangulation
  DynamicList<Face>  dynFaces(nElems);
  for (label faceI = 0; faceI < nElems; ++faceI)
  {
    line = this->getLineNoComment(is);
    {
      IStringStream lineStream(line);
      label nVerts;
      lineStream >> nVerts;
      List<label> verts(nVerts);
      FOR_ALL(verts, vertI)
      {
        lineStream >> verts[vertI];
      }
      labelUList& f = static_cast<labelUList&>(verts);
      if (mustTriangulate && f.size() > 3)
      {
        // simple face triangulation about f[0]
        // cannot use face::triangulation (points may be incomplete)
        for (label fp1 = 1; fp1 < f.size() - 1; fp1++)
        {
          label fp2 = f.fcIndex(fp1);
          dynFaces.append(triFace(f[0], f[fp1], f[fp2]));
        }
      }
      else
      {
        dynFaces.append(Face(f));
      }
    }
  }
  // transfer to normal lists, no zone information
  this->reset(pointLst.xfer(), dynFaces.xfer(), Xfer<surfZoneList>());
  return true;
}


template<class Face>
void mousse::fileFormats::OFFsurfaceFormat<Face>::write
(
  const fileName& filename,
  const MeshedSurfaceProxy<Face>& surf
)
{
  const pointField& pointLst = surf.points();
  const List<Face>&  faceLst = surf.faces();
  const List<label>& faceMap = surf.faceMap();
  const List<surfZone>& zoneLst = surf.surfZones();
  OFstream os{filename};
  if (!os.good())
  {
    FATAL_ERROR_IN
    (
      "fileFormats::OFFsurfaceFormat::write"
      "(const fileName&, const MeshedSurfaceProxy<Face>&)"
    )
    << "Cannot open file for writing " << filename
    << exit(FatalError);
  }
  // Write header
  os<< "OFF" << endl
    << "# Geomview OFF file written " << clock::dateTime().c_str() << nl
    << nl
    << "# points : " << pointLst.size() << nl
    << "# faces  : " << faceLst.size() << nl
    << "# zones  : " << zoneLst.size() << nl;
  // Print zone names as comment
  FOR_ALL(zoneLst, zoneI)
  {
    os<< "#   " << zoneI << "  " << zoneLst[zoneI].name()
      << "  (nFaces: " << zoneLst[zoneI].size() << ")" << nl;
  }
  os<< nl
    << "# nPoints  nFaces  nEdges" << nl
    << pointLst.size() << ' ' << faceLst.size() << ' ' << 0 << nl
    << nl
    << "# <points count=\"" << pointLst.size() << "\">" << endl;
  // Write vertex coords
  FOR_ALL(pointLst, ptI)
  {
    os<< pointLst[ptI].x() << ' '
      << pointLst[ptI].y() << ' '
      << pointLst[ptI].z() << " #" << ptI << endl;
  }
  os<< "# </points>" << nl
    << nl
    << "# <faces count=\"" << faceLst.size() << "\">" << endl;
  label faceIndex = 0;
  FOR_ALL(zoneLst, zoneI)
  {
    os << "# <zone name=\"" << zoneLst[zoneI].name() << "\">" << endl;
    if (surf.useFaceMap())
    {
      FOR_ALL(zoneLst[zoneI], localFaceI)
      {
        const Face& f = faceLst[faceMap[faceIndex++]];
        os << f.size();
        FOR_ALL(f, fp)
        {
          os << ' ' << f[fp];
        }
        // add optional zone information
        os << ' ' << zoneI << endl;
      }
    }
    else
    {
      FOR_ALL(zoneLst[zoneI], localFaceI)
      {
        const Face& f = faceLst[faceIndex++];
        os << f.size();
        FOR_ALL(f, fp)
        {
          os << ' ' << f[fp];
        }
        // add optional zone information
        os << ' ' << zoneI << endl;
      }
    }
    os << "# </zone>" << endl;
  }
  os << "# </faces>" << endl;
}
