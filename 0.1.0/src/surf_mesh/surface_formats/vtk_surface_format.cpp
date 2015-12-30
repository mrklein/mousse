// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vtk_surface_format.hpp"
#include "vtk_unstructured_reader.hpp"
#include "scalar_io_field.hpp"
// Private Member Functions 
template<class Face>
void mousse::fileFormats::VTKsurfaceFormat<Face>::writeHeaderPolygons
(
  Ostream& os,
  const UList<Face>& faceLst
)
{
  label nNodes = 0;
  forAll(faceLst, faceI)
  {
    nNodes += faceLst[faceI].size();
  }
  os  << nl
    << "POLYGONS " << faceLst.size() << ' '
    << faceLst.size() + nNodes << nl;
}
// Constructors 
template<class Face>
mousse::fileFormats::VTKsurfaceFormat<Face>::VTKsurfaceFormat
(
  const fileName& filename
)
{
  read(filename);
}
// Member Functions 
template<class Face>
bool mousse::fileFormats::VTKsurfaceFormat<Face>::read
(
  const fileName& filename
)
{
  const bool mustTriangulate = this->isTri();
  this->clear();
  IFstream is(filename);
  if (!is.good())
  {
    FatalErrorIn
    (
      "fileFormats::VTKsurfaceFormat::read(const fileName&)"
    )   << "Cannot read file " << filename
      << exit(FatalError);
  }
  // assume that the groups are not intermixed
  bool sorted = true;
  // Construct dummy time so we have something to create an objectRegistry
  // from
  Time dummyTime
  (
    "dummyRoot",
    "dummyCase",
    "system",
    "constant",
    false           // enableFunctionObjects
  );
  // Make dummy object registry
  objectRegistry obr
  (
    IOobject
    (
      "dummy",
      dummyTime,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    )
  );
  // Read all
  vtkUnstructuredReader reader(obr, is);
  const faceList& faces = reader.faces();
  // Assume all faces in zone0 unless a region field is present
  labelList zones(faces.size(), 0);
  if (reader.cellData().foundObject<scalarIOField>("region"))
  {
    const scalarIOField& region =
      reader.cellData().lookupObject<scalarIOField>
      (
        "region"
      );
    forAll(region, i)
    {
      zones[i] = label(region[i]);
    }
  }
  else if (reader.cellData().foundObject<scalarIOField>("STLSolidLabeling"))
  {
    const scalarIOField& region =
      reader.cellData().lookupObject<scalarIOField>
      (
        "STLSolidLabeling"
      );
    forAll(region, i)
    {
      zones[i] = label(region[i]);
    }
  }
  // Create zone names
  const label nZones = max(zones)+1;
  wordList zoneNames(nZones);
  forAll(zoneNames, i)
  {
    zoneNames[i] = "zone" + mousse::name(i);
  }
  // See if needs triangulation
  label nTri = 0;
  if (mustTriangulate)
  {
    forAll(faces, faceI)
    {
      nTri += faces[faceI].size()-2;
    }
  }
  if (nTri > 0)
  {
    DynamicList<Face> dynFaces(nTri);
    DynamicList<label> dynZones(nTri);
    forAll(faces, faceI)
    {
      const face& f = faces[faceI];
      for (label fp1 = 1; fp1 < f.size() - 1; fp1++)
      {
        label fp2 = f.fcIndex(fp1);
        dynFaces.append(triFace(f[0], f[fp1], f[fp2]));
        dynZones.append(zones[faceI]);
      }
    }
    // Count
    labelList zoneSizes(nZones, 0);
    forAll(dynZones, triI)
    {
      zoneSizes[dynZones[triI]]++;
    }
    this->sortFacesAndStore(dynFaces.xfer(), dynZones.xfer(), sorted);
    // add zones, culling empty ones
    this->addZones(zoneSizes, zoneNames, true);
  }
  else
  {
    DynamicList<Face> dynFaces(faces.size());
    forAll(faces, faceI)
    {
      const face& f = faces[faceI];
      dynFaces.append(Face(f));
    }
    // Count
    labelList zoneSizes(nZones, 0);
    forAll(zones, faceI)
    {
      zoneSizes[zones[faceI]]++;
    }
    this->sortFacesAndStore(dynFaces.xfer(), zones.xfer(), sorted);
    // add zones, culling empty ones
    this->addZones(zoneSizes, zoneNames, true);
  }
  // transfer to normal lists
  this->storedPoints().transfer(reader.points());
  return true;
}
template<class Face>
void mousse::fileFormats::VTKsurfaceFormat<Face>::write
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
  OFstream os(filename);
  if (!os.good())
  {
    FatalErrorIn
    (
      "fileFormats::VTKsurfaceFormat::write"
      "(const fileName&, const MeshedSurfaceProxy<Face>&)"
    )
      << "Cannot open file for writing " << filename
      << exit(FatalError);
  }
  writeHeader(os, pointLst);
  writeHeaderPolygons(os, faceLst);
  label faceIndex = 0;
  forAll(zones, zoneI)
  {
    const surfZone& zone = zones[zoneI];
    if (useFaceMap)
    {
      forAll(zone, localFaceI)
      {
        const Face& f = faceLst[faceMap[faceIndex++]];
        os << f.size();
        forAll(f, fp)
        {
          os << ' ' << f[fp];
        }
        os << ' ' << nl;
      }
    }
    else
    {
      forAll(zone, localFaceI)
      {
        const Face& f = faceLst[faceIndex++];
        os << f.size();
        forAll(f, fp)
        {
          os << ' ' << f[fp];
        }
        os << ' ' << nl;
      }
    }
  }
  writeTail(os, zones);
}
template<class Face>
void mousse::fileFormats::VTKsurfaceFormat<Face>::write
(
  const fileName& filename,
  const UnsortedMeshedSurface<Face>& surf
)
{
  OFstream os(filename);
  if (!os.good())
  {
    FatalErrorIn
    (
      "fileFormats::VTKsurfaceFormat::write"
      "(const fileName&, const UnsortedMeshedSurface<Face>&)"
    )
      << "Cannot open file for writing " << filename
      << exit(FatalError);
  }
  const List<Face>& faceLst = surf.faces();
  writeHeader(os, surf.points());
  writeHeaderPolygons(os, faceLst);
  forAll(faceLst, faceI)
  {
    const Face& f = faceLst[faceI];
    os << f.size();
    forAll(f, fp)
    {
      os << ' ' << f[fp];
    }
    os << ' ' << nl;
  }
  writeTail(os, surf.zoneIds());
}
