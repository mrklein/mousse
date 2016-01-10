// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef primitive_patch_hpp_
#define primitive_patch_hpp_
#include "_primitive_patch.hpp"
#include "face.hpp"
#include "sub_list.hpp"
#include "point_field.hpp"
namespace mousse
{
  typedef PrimitivePatch<face, SubList, const pointField&>
    primitivePatch;
}
#endif
