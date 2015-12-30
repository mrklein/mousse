// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef fv_patch_fields_fwd_hpp_
#define fv_patch_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
template<class Type> class fvPatchField;
typedef fvPatchField<scalar> fvPatchScalarField;
typedef fvPatchField<vector> fvPatchVectorField;
typedef fvPatchField<sphericalTensor> fvPatchSphericalTensorField;
typedef fvPatchField<symmTensor> fvPatchSymmTensorField;
typedef fvPatchField<tensor> fvPatchTensorField;
}  // namespace mousse
#endif
