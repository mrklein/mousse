// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef face_zone_mesh_fwd_hpp_
#define face_zone_mesh_fwd_hpp_
namespace mousse
{
  template<class Zone, class MeshType> class ZoneMesh;
  class faceZone;
  class polyMesh;
  typedef ZoneMesh<faceZone, polyMesh> faceZoneMesh;
}
#endif
