#ifndef FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_FWD_HPP_
#define FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"


namespace mousse {

template<class Type> class fvsPatchField;
typedef fvsPatchField<scalar> fvsPatchScalarField;
typedef fvsPatchField<vector> fvsPatchVectorField;
typedef fvsPatchField<sphericalTensor> fvsPatchSphericalTensorField;
typedef fvsPatchField<symmTensor> fvsPatchSymmTensorField;
typedef fvsPatchField<tensor> fvsPatchTensorField;

}  // namespace mousse

#endif
