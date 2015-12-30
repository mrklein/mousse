// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef b_mesh_hpp_
#define b_mesh_hpp_
#include "primitive_patch.hpp"
#include "face.hpp"
#include "list.hpp"
#include "point_field.hpp"
namespace mousse
{
  typedef PrimitivePatch<face, List, const pointField> bMesh;
}
#endif
