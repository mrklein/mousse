#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_DERIVED_CODED_FIXED_VALUE_POINT_PATCH_FIELDS_FWD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_DERIVED_CODED_FIXED_VALUE_POINT_PATCH_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"
namespace mousse
{
template<class Type> class codedFixedValuePointPatchField;
makePatchTypeFieldTypedefs(codedFixedValue);
}  // namespace mousse
#endif
