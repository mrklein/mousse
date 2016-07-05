#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_POINT_PATCH_FIELDS_FWD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_POINT_PATCH_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"


namespace mousse {

template<class Type> class pointPatchField;
typedef pointPatchField<scalar> pointPatchScalarField;
typedef pointPatchField<vector> pointPatchVectorField;
typedef pointPatchField<sphericalTensor> pointPatchSphericalTensorField;
typedef pointPatchField<symmTensor> pointPatchSymmTensorField;
typedef pointPatchField<tensor> pointPatchTensorField;

}  // namespace mousse

#endif
