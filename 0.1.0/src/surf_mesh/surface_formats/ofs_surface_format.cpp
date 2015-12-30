// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ofs_surface_format.hpp"
#include "ifstream.hpp"
#include "istring_stream.hpp"
#include "list_ops.hpp"
// Constructors 
template<class Face>
mousse::fileFormats::OFSsurfaceFormat<Face>::OFSsurfaceFormat
(
  const fileName& filename
)
{
  read(filename);
}
// Member Functions 
template<class Face>
bool mousse::fileFormats::OFSsurfaceFormat<Face>::read
(
  const fileName& filename
)
{
  this->clear();
  IFstream is(filename);
  if (!is.good())
  {
    FatalErrorIn
    (
      "fileFormats::OFSsurfaceFormat::read(const fileName&)"
    )
      << "Cannot read file " << filename
      << exit(FatalError);
  }
  // read surfZones:
  is >> this->storedZones();
  // read points:
  is >> this->storedPoints();
  // must triangulate?
  if (MeshedSurface<Face>::isTri())
  {
    // read faces as 'face' and transcribe to 'triFace'
    List<face> faceLst(is);
    MeshedSurface<face> surf
    (
      xferMove(this->storedPoints()),
      xferMove(faceLst),
      xferMove(this->storedZones())
    );
    this->transcribe(surf);
  }
  else
  {
    // read faces directly
    is >> this->storedFaces();
  }
  return true;
}
template<class Face>
bool mousse::fileFormats::OFSsurfaceFormat<Face>::read
(
  Istream& is,
  pointField& pointLst,
  List<Face>& faceLst,
  List<surfZone>& zoneLst
)
{
  if (!is.good())
  {
    FatalErrorIn
    (
      "fileFormats::OFSsurfaceFormat::read"
      "(Istream&, pointField&, List<Face>&, List<surfZone>&)"
    )
      << "read error "
      << exit(FatalError);
  }
  // read surfZones:
  is >> zoneLst;
  // read points:
  is >> pointLst;
  // must triangulate?
  if (MeshedSurface<Face>::isTri())
  {
    // read faces as 'face' and transcribe to 'triFace'
    List<face> origFaces(is);
    MeshedSurface<face> origSurf
    (
      xferMove(pointLst),
      xferMove(origFaces),
      xferMove(zoneLst)
    );
    MeshedSurface<Face> surf;
    surf.transcribe(origSurf);
  }
  else
  {
    // read faces directly
    is >> faceLst;
  }
  return true;
}
template<class Face>
bool mousse::fileFormats::OFSsurfaceFormat<Face>::read
(
  Istream& is,
  MeshedSurface<Face>& surf
)
{
  surf.clear();
  if (!is.good())
  {
    FatalErrorIn
    (
      "fileFormats::OFSsurfaceFormat::read"
      "(Istream&, MeshedSurface<Face>&)"
    )
      << "read error "
      << exit(FatalError);
  }
  pointField pointLst;
  List<Face> faceLst;
  List<surfZone> zoneLst;
  read(is, pointLst, faceLst, zoneLst);
  surf.reset
  (
    xferMove(pointLst),
    xferMove(faceLst),
    xferMove(zoneLst)
  );
  return true;
}
template<class Face>
bool mousse::fileFormats::OFSsurfaceFormat<Face>::read
(
  Istream& is,
  UnsortedMeshedSurface<Face>& surf
)
{
  surf.clear();
  MeshedSurface<Face> origSurf(is);
  surf.transfer(origSurf);
  return true;
}
template<class Face>
void mousse::fileFormats::OFSsurfaceFormat<Face>::write
(
  const fileName& filename,
  const MeshedSurfaceProxy<Face>& surf
)
{
  const List<Face>&  faceLst = surf.faces();
  const List<label>& faceMap = surf.faceMap();
  OFstream os(filename);
  if (!os.good())
  {
    FatalErrorIn
    (
      "fileFormats::OFSsurfaceFormat::write"
      "(const fileName&, const MeshedSurfaceProxy<Face>&)"
    )
      << "Cannot open file for writing " << filename
      << exit(FatalError);
  }
  OFSsurfaceFormatCore::writeHeader(os, surf.points(), surf.surfZones());
  const List<surfZone>& zones = surf.surfZones();
  const bool useFaceMap = (surf.useFaceMap() && zones.size() > 1);
  if (useFaceMap)
  {
    os  << "\n// faces:"  << nl
      << faceLst.size() << token::BEGIN_LIST << nl;
    label faceI = 0;
    forAll(zones, zoneI)
    {
      // Print all faces belonging to this zone
      const surfZone& zone = zones[zoneI];
      forAll(zone, localFaceI)
      {
        os << faceLst[faceMap[faceI++]] << nl;
      }
    }
    os << token::END_LIST << nl;
  }
  else
  {
    os  << "\n// faces:"  << nl << faceLst << nl;
  }
  IOobject::writeDivider(os);
  // Check state of Ostream
  os.check("OFSsurfaceFormat<Face>::write(Ostream&)");
}
