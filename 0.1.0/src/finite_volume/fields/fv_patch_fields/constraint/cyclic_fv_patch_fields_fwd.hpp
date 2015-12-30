// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef cyclic_fv_patch_fields_fwd_hpp_
#define cyclic_fv_patch_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
template<class Type> class cyclicFvPatchField;
makePatchTypeFieldTypedefs(cyclic);
}  // namespace mousse
#endif
