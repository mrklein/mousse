// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef fvs_patch_fields_fwd_hpp_
#define fvs_patch_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
template<class Type> class fvsPatchField;
typedef fvsPatchField<scalar> fvsPatchScalarField;
typedef fvsPatchField<vector> fvsPatchVectorField;
typedef fvsPatchField<sphericalTensor> fvsPatchSphericalTensorField;
typedef fvsPatchField<symmTensor> fvsPatchSymmTensorField;
typedef fvsPatchField<tensor> fvsPatchTensorField;
}  // namespace mousse
#endif
