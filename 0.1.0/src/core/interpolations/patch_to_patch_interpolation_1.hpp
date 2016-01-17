// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef patch_to_patch_interpolation_hpp_
#define patch_to_patch_interpolation_hpp_
#include "patch_to_patch_interpolation.hpp"
#include "primitive_patch.hpp"
#include "face.hpp"
#include "sub_list.hpp"
#include "point_field.hpp"
namespace mousse
{
  typedef PatchToPatchInterpolation
  <
    PrimitivePatch<face, SubList, const pointField&>,
    PrimitivePatch<face, SubList, const pointField&>
  >   patchToPatchInterpolation;
}
#endif
