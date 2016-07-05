#ifndef CORE_INTERPOLATIONS_PATCH_TO_PATCH_INTERPOLATION_HPP_
#define CORE_INTERPOLATIONS_PATCH_TO_PATCH_INTERPOLATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_patch_to_patch_interpolation.hpp"
#include "primitive_patch.hpp"
#include "face.hpp"
#include "sub_list.hpp"
#include "point_field.hpp"


namespace mousse {

typedef PatchToPatchInterpolation
<
  PrimitivePatch<face, SubList, const pointField&>,
  PrimitivePatch<face, SubList, const pointField&>
> patchToPatchInterpolation;

}

#endif  // CORE_INTERPOLATIONS_PATCH_TO_PATCH_INTERPOLATION_HPP_
