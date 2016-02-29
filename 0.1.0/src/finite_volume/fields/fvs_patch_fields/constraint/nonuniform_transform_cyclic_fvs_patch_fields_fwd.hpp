#ifndef FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_CONSTRAINT_NONUNIFORM_TRANSFORM_CYCLIC_FVS_PATCH_FIELDS_FWD_HPP_
#define FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_CONSTRAINT_NONUNIFORM_TRANSFORM_CYCLIC_FVS_PATCH_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"
namespace mousse
{
template<class Type> class nonuniformTransformCyclicFvsPatchField;
makeFvsPatchTypeFieldTypedefs(nonuniformTransformCyclic);
}  // namespace mousse
#endif
