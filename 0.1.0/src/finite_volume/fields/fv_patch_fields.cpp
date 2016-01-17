// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_patch_fields.hpp"

namespace mousse
{

// Static Data Members
MAKE_FV_PATCH_FIELD(fvPatchScalarField)
MAKE_FV_PATCH_FIELD(fvPatchVectorField)
MAKE_FV_PATCH_FIELD(fvPatchSphericalTensorField)
MAKE_FV_PATCH_FIELD(fvPatchSymmTensorField)
MAKE_FV_PATCH_FIELD(fvPatchTensorField)

}  // namespace mousse
