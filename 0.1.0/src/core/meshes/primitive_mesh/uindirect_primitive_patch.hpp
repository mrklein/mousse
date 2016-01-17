// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef uindirect_primitive_patch_hpp_
#define uindirect_primitive_patch_hpp_
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
