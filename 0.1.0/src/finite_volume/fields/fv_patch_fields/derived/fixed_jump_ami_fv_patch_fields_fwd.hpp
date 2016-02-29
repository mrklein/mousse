#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FIXED_JUMP_AMI_FV_PATCH_FIELDS_FWD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FIXED_JUMP_AMI_FV_PATCH_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"
namespace mousse
{
template<class Type> class fixedJumpAMIFvPatchField;
makePatchTypeFieldTypedefs(fixedJumpAMI);
}  // namespace mousse
#endif
