// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_patch_fields.hpp"
namespace mousse
{
// Static Data Members
makeFvPatchField(fvPatchScalarField)
makeFvPatchField(fvPatchVectorField)
makeFvPatchField(fvPatchSphericalTensorField)
makeFvPatchField(fvPatchSymmTensorField)
makeFvPatchField(fvPatchTensorField)
}  // namespace mousse
