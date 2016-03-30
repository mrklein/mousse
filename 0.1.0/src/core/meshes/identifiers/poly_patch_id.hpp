#ifndef CORE_MESHES_IDENTIFIERS_POLY_PATCH_ID_HPP_
#define CORE_MESHES_IDENTIFIERS_POLY_PATCH_ID_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_id.hpp"
#include "poly_boundary_mesh.hpp"


namespace mousse {

//- mousse::polyPatchID
typedef DynamicID<polyBoundaryMesh> polyPatchID;

}
#endif
