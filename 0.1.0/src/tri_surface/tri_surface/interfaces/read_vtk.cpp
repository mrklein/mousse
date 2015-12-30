// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
#include "vtk_surface_format.hpp"
namespace mousse
{
// Member Functions 
bool mousse::triSurface::readVTK(const fileName& fName)
{
  // Read (and triangulate) point, faces, zone info
  fileFormats::VTKsurfaceFormat<triFace> surf(fName);
  List<labelledTri> tris(surf.faces().size());
  forAll(tris, i)
  {
    const triFace& f = surf[i];
    tris[i] = labelledTri(f[0], f[1], f[2], 0);
  }
  // Add regions from zone
  const List<surfZone>& surfZones = surf.surfZones();
  geometricSurfacePatchList patches;
  if (surfZones.size())
  {
    patches.setSize(surfZones.size());
    forAll(surfZones, zoneI)
    {
      const surfZone& zone = surfZones[zoneI];
      // Add patch. Convert synthetic 'zone' name into 'patch' for now.
      // (vtk format does not contain region names)
      word regionName = zone.name();
      if (regionName != (string("zone") + name(zoneI)))
      {
        regionName = string("patch") + name(zoneI);
      }
      patches[zoneI] = geometricSurfacePatch
      (
        (
          zone.geometricType() != word::null
         ? zone.geometricType()
         : "empty"
        ),
        regionName,
        zoneI
      );
      // Set triangle regions
      for (label i = zone.start(); i < zone.start()+zone.size(); i++)
      {
        tris[i].region() = zoneI;
      }
    }
  }
  else
  {
    // Add single patch
    patches[0] = geometricSurfacePatch("empty", "patch0", 0);
    // Triangle regions already set to 0
  }
  // Create triSurface
  *this = triSurface
  (
    tris.xfer(),
    patches,
    xferCopy<List<point> >(surf.points())
  );
  return true;
}
}  // namespace mousse
