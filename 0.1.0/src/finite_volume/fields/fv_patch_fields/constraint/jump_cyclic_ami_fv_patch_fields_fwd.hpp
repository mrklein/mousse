#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_CONSTRAINT_JUMP_CYCLIC_AMI_FV_PATCH_FIELDS_FWD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_CONSTRAINT_JUMP_CYCLIC_AMI_FV_PATCH_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"


namespace mousse {

template<class Type> class jumpCyclicAMIFvPatchField;
MAKE_PATCH_TYPE_FIELD_TYPEDEFS(jumpCyclicAMI);

}  // namespace mousse

#endif

