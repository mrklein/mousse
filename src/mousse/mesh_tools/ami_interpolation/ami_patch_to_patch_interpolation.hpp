#ifndef MESH_TOOLS_AMI_INTERPOLATION_AMI_INTERPOLATION_AMI_PATCH_TO_PATCH_INTERPOLATION_HPP_
#define MESH_TOOLS_AMI_INTERPOLATION_AMI_INTERPOLATION_AMI_PATCH_TO_PATCH_INTERPOLATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ami_interpolation.hpp"
#include "primitive_patch.hpp"
#include "face.hpp"
#include "sub_list.hpp"
#include "point_field.hpp"


namespace mousse {

typedef AMIInterpolation
<
  PrimitivePatch<face, SubList, const pointField&>,
  PrimitivePatch<face, SubList, const pointField&>
> AMIPatchToPatchInterpolation;

}

#endif
