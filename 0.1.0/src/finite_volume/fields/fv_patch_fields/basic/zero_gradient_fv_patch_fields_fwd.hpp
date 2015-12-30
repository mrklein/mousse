// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef zero_gradient_fv_patch_fields_fwd_hpp_
#define zero_gradient_fv_patch_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
template<class Type> class zeroGradientFvPatchField;
makePatchTypeFieldTypedefs(zeroGradient);
}  // namespace mousse
#endif
