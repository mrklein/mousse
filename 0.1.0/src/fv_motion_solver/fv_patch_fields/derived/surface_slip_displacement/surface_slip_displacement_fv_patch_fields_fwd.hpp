#ifndef FV_MOTION_SOLVER_FV_PATCH_FIELDS_DERIVED_SURFACE_SLIP_DISPLACEMENT_SURFACE_SLIP_DISPLACEMENT_FV_PATCH_FIELDS_FWD_HPP_
#define FV_MOTION_SOLVER_FV_PATCH_FIELDS_DERIVED_SURFACE_SLIP_DISPLACEMENT_SURFACE_SLIP_DISPLACEMENT_FV_PATCH_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"
namespace mousse
{
template<class Type> class surfaceSlipDisplacementFvPatchField;
makePatchTypeFieldTypedefs(slip);
}  // namespace mousse
#endif
