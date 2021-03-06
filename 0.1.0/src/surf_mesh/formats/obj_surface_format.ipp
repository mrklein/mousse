// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "obj_surface_format.hpp"
#include "clock.hpp"
#include "ifstream.hpp"
#include "istring_stream.hpp"
#include "ostream.hpp"
#include "ofstream.hpp"
#include "list_ops.hpp"


// Constructors 
template<class Face>
mousse::fileFormats::OBJsurfaceFormat<Face>::OBJsurfaceFormat
(
  const fileName& filename
)
{
  read(filename);
}


// Member Functions 
template<class Face>
bool mousse::fileFormats::OBJsurfaceFormat<Face>::read
(
  const fileName& filename
)
{
  const bool mustTriangulate = this->isTri();
  this->clear();
  IFstream is{filename};
  if (!is.good()) {
    FATAL_ERROR_IN
    (
      "fileFormats::OBJsurfaceFormat::read(const fileName&)"
    )
    << "Cannot read file " << filename
    << exit(FatalError);
  }
  // assume that the groups are not intermixed
  bool sorted = true;
  DynamicList<point> dynPoints;
  DynamicList<Face> dynFaces;
  DynamicList<label> dynZones;
  DynamicList<word> dynNames;
  DynamicList<label> dynSizes;
  HashTable<label> lookup;
  // place faces without a group in zone0
  label zoneI = 0;
  lookup.insert("zone0", zoneI);
  dynNames.append("zone0");
  dynSizes.append(0);
  while (is.good()) {
    string line = this->getLineNoComment(is);
    // handle continuations
    if (line[line.size()-1] == '\\') {
      line.substr(0, line.size()-1);
      line += this->getLineNoComment(is);
    }
    // Read first word
    IStringStream lineStream{line};
    word cmd;
    lineStream >> cmd;
    if (cmd == "v") {
      scalar x, y, z;
      lineStream >> x >> y >> z;
      dynPoints.append(point(x, y, z));
    } else if (cmd == "g") {
      word name;
      lineStream >> name;
      HashTable<label>::const_iterator fnd = lookup.find(name);
      if (fnd != lookup.end()) {
        if (zoneI != fnd()) {
          // group appeared out of order
          sorted = false;
        }
        zoneI = fnd();
      } else {
        zoneI = dynSizes.size();
        lookup.insert(name, zoneI);
        dynNames.append(name);
        dynSizes.append(0);
      }
    } else if (cmd == "f") {
      DynamicList<label> dynVertices;
      // Assume 'f' is followed by space.
      string::size_type endNum = 1;
      while (true) {
        string::size_type startNum = line.find_first_not_of(' ', endNum);
        if (startNum == string::npos) {
          break;
        }
        endNum = line.find(' ', startNum);
        string vertexSpec;
        if (endNum != string::npos) {
          vertexSpec = line.substr(startNum, endNum-startNum);
        } else {
          vertexSpec = line.substr(startNum, line.size() - startNum);
        }
        string::size_type slashPos = vertexSpec.find('/');
        label vertI = 0;
        if (slashPos != string::npos) {
          IStringStream intStream{vertexSpec.substr(0, slashPos)};
          intStream >> vertI;
        }
        else
        {
          IStringStream intStream(vertexSpec);
          intStream >> vertI;
        }
        dynVertices.append(vertI - 1);
      }
      dynVertices.shrink();
      labelUList& f = static_cast<labelUList&>(dynVertices);
      if (mustTriangulate && f.size() > 3)
      {
        // simple face triangulation about f[0]
        // points may be incomplete
        for (label fp1 = 1; fp1 < f.size() - 1; fp1++)
        {
          label fp2 = f.fcIndex(fp1);
          dynFaces.append(triFace(f[0], f[fp1], f[fp2]));
          dynZones.append(zoneI);
          dynSizes[zoneI]++;
        }
      }
      else
      {
        dynFaces.append(Face(f));
        dynZones.append(zoneI);
        dynSizes[zoneI]++;
      }
    }
  }
  // transfer to normal lists
  this->storedPoints().transfer(dynPoints);
  this->sortFacesAndStore(dynFaces.xfer(), dynZones.xfer(), sorted);
  // add zones, culling empty ones
  this->addZones(dynSizes, dynNames, true);
  return true;
}


template<class Face>
void mousse::fileFormats::OBJsurfaceFormat<Face>::write
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
      "fileFormats::OBJsurfaceFormat::write"
      "(const fileName&, const MeshedSurfaceProxy<Face>&)"
    )
    << "Cannot open file for writing " << filename
    << exit(FatalError);
  }
  os << "# Wavefront OBJ file written " << clock::dateTime().c_str() << nl
    << "o " << os.name().lessExt().name() << nl
    << nl
    << "# points : " << pointLst.size() << nl
    << "# faces  : " << faceLst.size() << nl
    << "# zones  : " << zones.size() << nl;
  // Print zone names as comment
  FOR_ALL(zones, zoneI)
  {
    os << "#   " << zoneI << "  " << zones[zoneI].name()
      << "  (nFaces: " << zones[zoneI].size() << ")" << nl;
  }
  os << nl << "# <points count=\"" << pointLst.size() << "\">" << nl;
  // Write vertex coords
  FOR_ALL(pointLst, ptI)
  {
    const point& pt = pointLst[ptI];
    os  << "v " << pt.x() << ' '  << pt.y() << ' '  << pt.z() << nl;
  }
  os  << "# </points>" << nl
    << nl
    << "# <faces count=\"" << faceLst.size() << "\">" << endl;
  label faceIndex = 0;
  FOR_ALL(zones, zoneI)
  {
    const surfZone& zone = zones[zoneI];
    if (zone.name().size())
    {
      os << "g " << zone.name() << endl;
    }
    if (useFaceMap)
    {
      FOR_ALL(zone, localFaceI)
      {
        const Face& f = faceLst[faceMap[faceIndex++]];
        os << 'f';
        FOR_ALL(f, fp)
        {
          os << ' ' << f[fp] + 1;
        }
        os << endl;
      }
    }
    else
    {
      FOR_ALL(zone, localFaceI)
      {
        const Face& f = faceLst[faceIndex++];
        os << 'f';
        FOR_ALL(f, fp)
        {
          os << ' ' << f[fp] + 1;
        }
        os << endl;
      }
    }
  }
  os << "# </faces>" << endl;
}
