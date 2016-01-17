// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef primitive_patch_interpolation_hpp_
#define primitive_patch_interpolation_hpp_
#include "primitive_patch_interpolation.hpp"
#include "primitive_patch.hpp"
#include "face.hpp"
#include "sub_list.hpp"
#include "point_field.hpp"
namespace mousse
{
  typedef PrimitivePatchInterpolation
  <
    PrimitivePatch<face, SubList, const pointField&>
  >   primitivePatchInterpolation;
}
#endif
