// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef cell_zone_mesh_fwd_hpp_
#define cell_zone_mesh_fwd_hpp_
namespace mousse
{
  template<class Zone, class MeshType> class ZoneMesh;
  class cellZone;
  class polyMesh;
  typedef ZoneMesh<cellZone, polyMesh> cellZoneMesh;
}
#endif
