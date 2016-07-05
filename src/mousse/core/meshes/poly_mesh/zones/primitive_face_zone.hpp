#ifndef CORE_MESHES_POLY_MESH_ZONES_PRIMITIVE_FACE_ZONE_HPP_
#define CORE_MESHES_POLY_MESH_ZONES_PRIMITIVE_FACE_ZONE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face.hpp"
#include "primitive_patch.hpp"
#include "indirect_face_list.hpp"
#include "point_field.hpp"


namespace mousse {

typedef PrimitivePatch<face, IndirectList, const pointField&>
  primitiveFaceZone;

}

#endif

