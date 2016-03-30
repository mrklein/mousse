#ifndef CORE_INTERPOLATIONS_PRIMITIVE_PATCH_INTERPOLATION_HPP_
#define CORE_INTERPOLATIONS_PRIMITIVE_PATCH_INTERPOLATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_primitive_patch_interpolation.hpp"
#include "primitive_patch.hpp"
#include "face.hpp"
#include "sub_list.hpp"
#include "point_field.hpp"


namespace mousse {

typedef PrimitivePatchInterpolation
<
  PrimitivePatch<face, SubList, const pointField&>
> primitivePatchInterpolation;

}

#endif
