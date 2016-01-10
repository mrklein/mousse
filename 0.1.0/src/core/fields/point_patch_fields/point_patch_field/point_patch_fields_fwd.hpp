// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef point_patch_fields_fwd_hpp_
#define point_patch_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
template<class Type> class pointPatchField;
typedef pointPatchField<scalar> pointPatchScalarField;
typedef pointPatchField<vector> pointPatchVectorField;
typedef pointPatchField<sphericalTensor> pointPatchSphericalTensorField;
typedef pointPatchField<symmTensor> pointPatchSymmTensorField;
typedef pointPatchField<tensor> pointPatchTensorField;
}  // namespace mousse
#endif
