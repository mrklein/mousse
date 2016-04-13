// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvs_patch_fields.hpp"


namespace mousse {

// Static Data Members
#define MAKE_FVS_PATCH_FIELD(fvsPatchTypeField)                               \
                                                                              \
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(fvsPatchTypeField, 0);              \
template<>                                                                    \
int fvsPatchTypeField::disallowGenericFvsPatchField                           \
(                                                                             \
  debug::debugSwitch("disallowGenericFvsPatchField", 0)                       \
);                                                                            \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(fvsPatchTypeField, patch);           \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(fvsPatchTypeField, patchMapper);     \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(fvsPatchTypeField, dictionary);

MAKE_FVS_PATCH_FIELD(fvsPatchScalarField)
MAKE_FVS_PATCH_FIELD(fvsPatchVectorField)
MAKE_FVS_PATCH_FIELD(fvsPatchSphericalTensorField)
MAKE_FVS_PATCH_FIELD(fvsPatchSymmTensorField)
MAKE_FVS_PATCH_FIELD(fvsPatchTensorField)

}  // namespace mousse
