#ifndef DYNAMIC_MESH_BOUNDARY_MESH_B_MESH_HPP_
#define DYNAMIC_MESH_BOUNDARY_MESH_B_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_primitive_patch.hpp"
#include "face.hpp"
#include "list.hpp"
#include "point_field.hpp"


namespace mousse {

typedef PrimitivePatch<face, List, const pointField> bMesh;

}

#endif

