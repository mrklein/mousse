// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef cyclic_fvs_patch_fields_fwd_hpp_
#define cyclic_fvs_patch_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
template<class Type> class cyclicFvsPatchField;
makeFvsPatchTypeFieldTypedefs(cyclic);
}  // namespace mousse
#endif
