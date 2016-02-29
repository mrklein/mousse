#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_INLET_OUTLET_FV_PATCH_FIELDS_FWD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_INLET_OUTLET_FV_PATCH_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"
namespace mousse
{
template<class Type> class uniformInletOutletFvPatchField;
makePatchTypeFieldTypedefs(uniformInletOutlet);
}  // namespace mousse
#endif
