#ifndef CORE_MESHES_POLY_MESH_ZONES_POINT_ZONE_MESH_FWD_HPP_
#define CORE_MESHES_POLY_MESH_ZONES_POINT_ZONE_MESH_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse {

template<class Zone, class MeshType> class ZoneMesh;
class pointZone;
class polyMesh;
typedef ZoneMesh<pointZone, polyMesh> pointZoneMesh;

}

#endif

