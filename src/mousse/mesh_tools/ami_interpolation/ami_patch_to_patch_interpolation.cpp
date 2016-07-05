// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ami_patch_to_patch_interpolation.hpp"
#include "ami_method.hpp"
#include "direct_ami.hpp"
#include "map_nearest_ami.hpp"
#include "face_area_weight_ami.hpp"
#include "partial_face_area_weight_ami.hpp"


namespace mousse
{

MAKE_AMI_METHOD(AMIPatchToPatchInterpolation);
MAKE_AMI_METHOD_TYPE(AMIPatchToPatchInterpolation, directAMI);
MAKE_AMI_METHOD_TYPE(AMIPatchToPatchInterpolation, mapNearestAMI);
MAKE_AMI_METHOD_TYPE(AMIPatchToPatchInterpolation, faceAreaWeightAMI);
MAKE_AMI_METHOD_TYPE(AMIPatchToPatchInterpolation, partialFaceAreaWeightAMI);

}
