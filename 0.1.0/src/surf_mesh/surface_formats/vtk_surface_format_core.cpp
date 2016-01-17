// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vtk_surface_format_core.hpp"
#include "clock.hpp"
// Private Member Functions 
void mousse::fileFormats::VTKsurfaceFormatCore::writeHeader
(
  Ostream& os,
  const pointField& pointLst
)
{
  // Write header
  os  << "# vtk DataFile Version 2.0" << nl
    << "surface written " << clock::dateTime().c_str() << nl
    << "ASCII" << nl
    << nl
    << "DATASET POLYDATA" << nl;
  // Write vertex coords
  os  << "POINTS " << pointLst.size() << " float" << nl;
  FOR_ALL(pointLst, ptI)
  {
    const point& pt = pointLst[ptI];
    os  << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
  }
}
void mousse::fileFormats::VTKsurfaceFormatCore::writeTail
(
  Ostream& os,
  const UList<surfZone>& zoneLst
)
{
  label nFaces = 0;
  FOR_ALL(zoneLst, zoneI)
  {
    nFaces += zoneLst[zoneI].size();
  }
  // Print zone numbers
  os  << nl
    << "CELL_DATA " << nFaces << nl
    << "FIELD attributes 1" << nl
    << "region 1 " << nFaces << " float" << nl;
  FOR_ALL(zoneLst, zoneI)
  {
    FOR_ALL(zoneLst[zoneI], localFaceI)
    {
      if (localFaceI)
      {
        if (localFaceI % 20)
        {
          os << ' ';
        }
        else
        {
          os << nl;
        }
      }
      os  << zoneI + 1;
    }
    os  << nl;
  }
}
void mousse::fileFormats::VTKsurfaceFormatCore::writeTail
(
  Ostream& os,
  const labelUList& zoneIds
)
{
  // Print zone numbers
  os<< nl
    << "CELL_DATA " << zoneIds.size() << nl
    << "FIELD attributes 1" << nl
    << "region 1 " << zoneIds.size() << " float" << nl;
  FOR_ALL(zoneIds, faceI)
  {
    if (faceI)
    {
      if (faceI % 20)
      {
        os << ' ';
      }
      else
      {
        os << nl;
      }
    }
    os<< zoneIds[faceI] + 1;
  }
  os<< nl;
}
