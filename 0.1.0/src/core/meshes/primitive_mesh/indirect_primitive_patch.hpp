// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef indirect_primitive_patch_hpp_
#define indirect_primitive_patch_hpp_
#include "_primitive_patch.hpp"
#include "face.hpp"
#include "indirect_list.hpp"
#include "point_field.hpp"
namespace mousse
{
  typedef PrimitivePatch<face, IndirectList, const pointField&>
    indirectPrimitivePatch;
}
#endif
