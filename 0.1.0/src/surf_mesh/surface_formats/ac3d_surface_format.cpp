// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ac3d_surface_format.hpp"

#include "clock.hpp"
#include "istring_stream.hpp"
#include "tensor.hpp"
#include "primitive_patch.hpp"

// Constructors 
template<class Face>
mousse::fileFormats::AC3DsurfaceFormat<Face>::AC3DsurfaceFormat
(
  const fileName& filename
)
{
  read(filename);
}


// Member Functions 
template<class Face>
bool mousse::fileFormats::AC3DsurfaceFormat<Face>::read
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
      "fileFormats::AC3DsurfaceFormat::read(const fileName&)"
    )
    << "Cannot read file " << filename
    << exit(FatalError);
  }
  string line, cmd, args;
  is.getLine(line);
  string version = line.substr(4);
  if (version != "b")
  {
    WARNING_IN
    (
      "fileFormats::AC3DsurfaceFormat::read(const fileName&)"
    )
    << "When reading AC3D file " << filename
    << " read header " << line << " with version "
    << version << endl
    << "Only tested reading with version 'b'."
    << " This might give problems" << endl;
  }
  if (!cueTo(is, "OBJECT", args) || (args != "world"))
  {
    FATAL_ERROR_IN
    (
      "fileFormats::AC3DsurfaceFormat::read(const fileName&)"
    )
    << "Cannot find \"OBJECT world\" in file " << filename
    << exit(FatalError);
  }
  // # of kids is the # of zones
  args = cueToOrDie(is, "kids");
  label nZones = parse<int>(args);
  // Start of vertices for object/zones
  label vertexOffset = 0;
  DynamicList<point> dynPoints;
  DynamicList<Face> dynFaces;
  List<word> names{nZones};
  List<label> sizes{nZones, 0};
  for (label zoneI = 0; zoneI < nZones; ++zoneI)
  {
    names[zoneI] = word("zone") + mousse::name(zoneI);
    args = cueToOrDie(is, "OBJECT", "while reading " + names[zoneI]);
    // number of vertices for this zone
    label  nZonePoints = 0;
    vector location(pTraits<vector>::zero);
    // tensor rotation(I);
    // Read all info for current zone
    while (is.good())
    {
      // Read line and get first word. If end of file break since
      // zone should always end with 'kids' command ?not sure.
      if (!readCmd(is, cmd, args))
      {
        FATAL_ERROR_IN
        (
          "fileFormats::AC3DsurfaceFormat::read(const fileName&)"
        )
        << "Did not read up to \"kids 0\" while reading zone "
        << zoneI << " from file " << filename
        << exit(FatalError);
      }
      if (cmd == "name")
      {
        // name %s
        string str = parse<string>(args);
        string::stripInvalid<word>(str);
        names[zoneI] = str;
      }
      else if (cmd == "rot")
      {
        WARNING_IN
        (
          "fileFormats::AC3DsurfaceFormat::read"
          "(const fileName&)"
        )
        << "rot (rotation tensor) command not implemented"
        << "Line:" << cmd << ' ' << args << endl
        << "while reading zone " << zoneI << endl;
      }
      else if (cmd == "loc")
      {
        // loc  %f %f %f
        IStringStream lineStream(args);
        lineStream
          >> location.x()
          >> location.y()
          >> location.z();
      }
      else if (cmd == "numvert")
      {
        // numvert  %d
        nZonePoints = parse<int>(args);
        for (label vertI = 0; vertI < nZonePoints; ++vertI)
        {
          is.getLine(line);
          IStringStream lineStream(line);
          point pt;
          lineStream >> pt.x() >> pt.y() >> pt.z();
          // Offset with current translation vector
          dynPoints.append(location + pt);
        }
      }
      else if (cmd == "numsurf")
      {
        label nFaces = parse<int>(args);
        for (label faceI = 0; faceI < nFaces; ++faceI)
        {
          static string errorMsg =
            string(" while reading face ")
              + mousse::name(faceI) + " on zone "
              + mousse::name(zoneI)
              + " from file " + filename;
          cueToOrDie(is, "SURF", errorMsg);
          cueToOrDie(is, "mat", errorMsg);
          args = cueToOrDie(is, "refs", errorMsg);
          label nVert = parse<int>(args);
          List<label> verts(nVert);
          FOR_ALL(verts, vertI)
          {
            is.getLine(line);
            verts[vertI] = parse<int>(line) + vertexOffset;
          }
          labelUList& f = static_cast<labelUList&>(verts);
          if (mustTriangulate && f.size() > 3)
          {
            // simple face triangulation about f[0]
            // points may be incomplete
            for (label fp1 = 1; fp1 < f.size() - 1; ++fp1)
            {
              label fp2 = f.fcIndex(fp1);
              dynFaces.append(triFace(f[0], f[fp1], f[fp2]));
              sizes[zoneI]++;
            }
          }
          else
          {
            dynFaces.append(Face(f));
            sizes[zoneI]++;
          }
        }
        // Done the current zone.
        // Increment the offset vertices are stored at
        vertexOffset += nZonePoints;
      }
      else if (cmd == "kids")
      {
        // 'kids' denotes the end of the current zone.
        label nKids = parse<int>(args);
        if (nKids != 0)
        {
          FATAL_ERROR_IN
          (
            "fileFormats::AC3DsurfaceFormat::read(const fileName&)"
          )
          << "Can only read objects without kids."
          << " Encountered " << nKids << " kids when"
          << " reading zone " << zoneI
          << exit(FatalError);
        }
        // Done reading current zone
        break;
      }
    }
  }
  // transfer to normal lists
  this->storedPoints().transfer(dynPoints);
  this->storedFaces().transfer(dynFaces);
  // add zones, culling empty ones
  this->addZones(sizes, names, true);
  this->stitchFaces(SMALL);
  return true;
}


template<class Face>
void mousse::fileFormats::AC3DsurfaceFormat<Face>::write
(
  const fileName& filename,
  const MeshedSurfaceProxy<Face>& surf
)
{
  const pointField& pointLst = surf.points();
  const List<Face>&  faceLst = surf.faces();
  const List<surfZone>& zones =
  (
    surf.surfZones().size()
   ? surf.surfZones()
   : surfaceFormatsCore::oneZone(faceLst)
  );
  const bool useFaceMap = (surf.useFaceMap() && zones.size() > 1);
  if (useFaceMap)
  {
    FATAL_ERROR_IN
    (
      "fileFormats::AC3DsurfaceFormat::write"
      "(const fileName&, const MeshedSurfaceProxy<Face>&)"
    )
    << "output with faceMap is not supported " << filename
    << exit(FatalError);
  }
  OFstream os{filename};
  if (!os.good())
  {
    FATAL_ERROR_IN
    (
      "fileFormats::AC3DsurfaceFormat::write"
      "(const fileName&, const MeshedSurfaceProxy<Face>&)"
    )
    << "Cannot open file for writing " << filename
    << exit(FatalError);
  }
  writeHeader(os, zones);
  FOR_ALL(zones, zoneI)
  {
    const surfZone& zone = zones[zoneI];
    os  << "OBJECT poly" << nl
      << "name \"" << zone.name() << "\"\n";
    // Temporary PrimitivePatch to calculate compact points & faces
    // use 'UList' to avoid allocations!
    PrimitivePatch<Face, UList, const pointField&> patch
    {
      SubList<Face>
      {
        faceLst,
        zone.size(),
        zone.start()
      },
      pointLst
    };
    os << "numvert " << patch.nPoints() << endl;
    FOR_ALL(patch.localPoints(), ptI)
    {
      const point& pt = patch.localPoints()[ptI];
      os << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
    }
    os << "numsurf " << patch.localFaces().size() << endl;
    FOR_ALL(patch.localFaces(), localFaceI)
    {
      const Face& f = patch.localFaces()[localFaceI];
      os<< "SURF 0x20" << nl          // polygon
        << "mat " << zoneI << nl
        << "refs " << f.size() << nl;
      FOR_ALL(f, fp)
      {
        os << f[fp] << " 0 0" << nl;
      }
    }
    os << "kids 0" << endl;
  }
}


template<class Face>
void mousse::fileFormats::AC3DsurfaceFormat<Face>::write
(
  const fileName& filename,
  const UnsortedMeshedSurface<Face>& surf
)
{
  labelList faceMap;
  List<surfZone> zoneLst = surf.sortedZones(faceMap);
  if (zoneLst.size() <= 1)
  {
    write
    (
      filename,
      MeshedSurfaceProxy<Face>
      (
        surf.points(),
        surf.faces(),
        zoneLst
      )
    );
  }
  else
  {
    OFstream os{filename};
    if (!os.good())
    {
      FATAL_ERROR_IN
      (
        "fileFormats::AC3DsurfaceFormat::write"
        "(const fileName&, const MeshedSurfaceProxy<Face>&)"
      )
      << "Cannot open file for writing " << filename
      << exit(FatalError);
    }
    writeHeader(os, zoneLst);
    label faceIndex = 0;
    FOR_ALL(zoneLst, zoneI)
    {
      const surfZone& zone = zoneLst[zoneI];
      os<< "OBJECT poly" << nl
        << "name \"" << zone.name() << "\"\n";
      // Create zone with only zone faces included for ease of addressing
      labelHashSet include(surf.size());
      FOR_ALL(zone, localFaceI)
      {
        const label faceI = faceMap[faceIndex++];
        include.insert(faceI);
      }
      UnsortedMeshedSurface<Face> subm = surf.subsetMesh(include);
      // Now we have isolated surface for this patch alone. Write it.
      os << "numvert " << subm.nPoints() << endl;
      FOR_ALL(subm.localPoints(), ptI)
      {
        const point& pt = subm.localPoints()[ptI];
        os << pt.x() << ' ' << pt.y() << ' ' << pt.z() << endl;
      }
      os << "numsurf " << subm.localFaces().size() << endl;
      FOR_ALL(subm.localFaces(), localFaceI)
      {
        const Face& f = subm.localFaces()[localFaceI];
        os  << "SURF 0x20" << nl          // polygon
          << "mat " << zoneI << nl
          << "refs " << f.size() << nl;
        FOR_ALL(f, fp)
        {
          os << f[fp] << " 0 0" << nl;
        }
      }
      os << "kids 0" << endl;
    }
  }
}
