// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_patch_fields.hpp"
namespace mousse
{
// Static Data Members
#define MAKE_POINT_PATCH_FIELD(pointPatchTypeField)                           \
                                                                              \
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(pointPatchTypeField, 0);            \
template<>                                                                    \
int pointPatchTypeField::disallowGenericPointPatchField                       \
(                                                                             \
  debug::debugSwitch("disallowGenericPointPatchField", 0)                     \
);                                                                            \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(pointPatchTypeField, pointPatch);    \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(pointPatchTypeField, patchMapper);   \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(pointPatchTypeField, dictionary);

MAKE_POINT_PATCH_FIELD(pointPatchScalarField)
MAKE_POINT_PATCH_FIELD(pointPatchVectorField)
MAKE_POINT_PATCH_FIELD(pointPatchSphericalTensorField)
MAKE_POINT_PATCH_FIELD(pointPatchSymmTensorField)
MAKE_POINT_PATCH_FIELD(pointPatchTensorField)
}  // namespace mousse
