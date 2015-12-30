// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MeshedSurfaceIOAllocator
// Description
//   A helper class for storing points, faces and zones with IO capabilities.
// SourceFiles
//   meshed_surface_io_allocator.cpp
#ifndef meshed_surface_io_allocator_hpp_
#define meshed_surface_io_allocator_hpp_
#include "point_io_field.hpp"
#include "face_io_list.hpp"
#include "surf_zone_io_list.hpp"
namespace mousse
{
class MeshedSurfaceIOAllocator
{
  // Private data
    //- Points
    pointIOField points_;
    //- Faces
    faceCompactIOList faces_;
    //- Surface zones
    surfZoneIOList zones_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    MeshedSurfaceIOAllocator(const MeshedSurfaceIOAllocator&);
    //- Disallow default bitwise assignment
    void operator=(const MeshedSurfaceIOAllocator&);
public:
  // Constructors
    //- Read construct from IOobjects
    MeshedSurfaceIOAllocator
    (
      const IOobject& ioPoints,
      const IOobject& ioFaces,
      const IOobject& ioZones
    );
    //- Construct from IOobjects, copying components
    MeshedSurfaceIOAllocator
    (
      const IOobject& ioPoints,
      const pointField& points,
      const IOobject& ioFaces,
      const faceList& faces,
      const IOobject& ioZones,
      const surfZoneList& zones
    );
    //- Construct from IOobjects, possibly transferring components
    MeshedSurfaceIOAllocator
    (
      const IOobject& ioPoints,
      const Xfer<pointField>& points,
      const IOobject& ioFaces,
      const Xfer<faceList>& faces,
      const IOobject& ioZones,
      const Xfer<surfZoneList>& zones
    );
  // Member Functions
    // Access
      //- Non-const access to the points
      pointIOField& storedIOPoints()
      {
        return points_;
      }
      //- Non-const access to the faces
      faceCompactIOList& storedIOFaces()
      {
        return faces_;
      }
      //- Non-const access to the zones
      surfZoneIOList& storedIOZones()
      {
        return zones_;
      }
      //- Const access to the points
      const pointIOField& storedIOPoints() const
      {
        return points_;
      }
      //- Const access to the faces
      const faceCompactIOList& storedIOFaces() const
      {
        return faces_;
      }
      //- Const access to the zones
      const surfZoneIOList& storedIOZones() const
      {
        return zones_;
      }
    //  Storage management
      //- Clear primitive data (points, faces and zones)
      void clear();
      //- Reset primitive data (points, faces and zones)
      //  Note, optimized to avoid overwriting data (with Xfer::null)
      void resetFaces
      (
        const Xfer<faceList>& faces,
        const Xfer<surfZoneList>& zones
      );
      //- Reset primitive data (points, faces and zones)
      //  Note, optimized to avoid overwriting data (with Xfer::null)
      void reset
      (
        const Xfer<pointField>& points,
        const Xfer<faceList>& faces,
        const Xfer<surfZoneList>& zones
      );
      //- Reset primitive data (points, faces and zones)
      //  Note, optimized to avoid overwriting data (with Xfer::null)
      void reset
      (
        const Xfer<List<point> >& points,
        const Xfer<faceList>& faces,
        const Xfer<surfZoneList>& zones
      );
};
}  // namespace mousse
#endif
