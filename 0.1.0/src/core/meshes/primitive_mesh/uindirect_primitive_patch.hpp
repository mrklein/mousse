#ifndef CORE_MESHES_PRIMITIVE_MESH_UINDIRECT_PRIMITIVE_PATCH_HPP_
#define CORE_MESHES_PRIMITIVE_MESH_UINDIRECT_PRIMITIVE_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_primitive_patch.hpp"
#include "face.hpp"
#include "uindirect_list.hpp"
#include "point_field.hpp"
namespace mousse
{
  typedef PrimitivePatch<face, UIndirectList, const pointField&>
    uindirectPrimitivePatch;
}
#endif
