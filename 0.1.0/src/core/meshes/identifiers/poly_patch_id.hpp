// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef poly_patch_id_hpp_
#define poly_patch_id_hpp_
#include "dynamic_id.hpp"
#include "poly_boundary_mesh.hpp"
namespace mousse
{
  //- mousse::polyPatchID
  typedef DynamicID<polyBoundaryMesh> polyPatchID;
}
#endif
