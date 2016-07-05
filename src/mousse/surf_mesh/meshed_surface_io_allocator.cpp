// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "meshed_surface_io_allocator.hpp"


// Constructors 
mousse::MeshedSurfaceIOAllocator::MeshedSurfaceIOAllocator
(
  const IOobject& ioPoints,
  const IOobject& ioFaces,
  const IOobject& ioZones
)
:
  points_{ioPoints},
  faces_{ioFaces},
  zones_{ioZones}
{}


mousse::MeshedSurfaceIOAllocator::MeshedSurfaceIOAllocator
(
  const IOobject& ioPoints,
  const pointField& points,
  const IOobject& ioFaces,
  const faceList& faces,
  const IOobject& ioZones,
  const surfZoneList& zones
)
:
  points_{ioPoints, points},
  faces_{ioFaces, faces},
  zones_{ioZones, zones}
{}


mousse::MeshedSurfaceIOAllocator::MeshedSurfaceIOAllocator
(
  const IOobject& ioPoints,
  const Xfer<pointField>& points,
  const IOobject& ioFaces,
  const Xfer<faceList>& faces,
  const IOobject& ioZones,
  const Xfer<surfZoneList>& zones
)
:
  points_{ioPoints, points},
  faces_{ioFaces, faces},
  zones_{ioZones, zones}
{}


// Member Functions 
void mousse::MeshedSurfaceIOAllocator::clear()
{
  points_.clear();
  faces_.clear();
  zones_.clear();
}


void mousse::MeshedSurfaceIOAllocator::resetFaces
(
  const Xfer<List<face> >& faces,
  const Xfer<surfZoneList>& zones
)
{
  if (notNull(faces)) {
    faces_.transfer(faces());
  }
  if (notNull(zones)) {
    zones_.transfer(zones());
  }
}


void mousse::MeshedSurfaceIOAllocator::reset
(
  const Xfer<pointField>& points,
  const Xfer<faceList>& faces,
  const Xfer<surfZoneList>& zones
)
{
  // Take over new primitive data.
  // Optimized to avoid overwriting data at all
  if (notNull(points)) {
    points_.transfer(points());
  }
  resetFaces(faces, zones);
}


void mousse::MeshedSurfaceIOAllocator::reset
(
  const Xfer<List<point> >& points,
  const Xfer<faceList>& faces,
  const Xfer<surfZoneList>& zones
)
{
  // Take over new primitive data.
  // Optimized to avoid overwriting data at all
  if (notNull(points)) {
    points_.transfer(points());
  }
  resetFaces(faces, zones);
}
