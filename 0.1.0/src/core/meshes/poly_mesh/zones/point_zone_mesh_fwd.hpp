// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef point_zone_mesh_fwd_hpp_
#define point_zone_mesh_fwd_hpp_
namespace mousse
{
  template<class Zone, class MeshType> class ZoneMesh;
  class pointZone;
  class polyMesh;
  typedef ZoneMesh<pointZone, polyMesh> pointZoneMesh;
}
#endif
