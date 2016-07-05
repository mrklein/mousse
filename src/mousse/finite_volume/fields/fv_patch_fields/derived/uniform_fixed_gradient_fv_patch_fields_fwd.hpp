#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_FIXED_GRADIENT_FV_PATCH_FIELDS_FWD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_FIXED_GRADIENT_FV_PATCH_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"


namespace mousse {

template<class Type> class uniformFixedGradientFvPatchField;
MAKE_PATCH_TYPE_FIELD_TYPEDEFS(uniformFixedGradient);

}  // namespace mousse

#endif

