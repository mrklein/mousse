// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef inlet_outlet_fv_patch_fields_fwd_hpp_
#define inlet_outlet_fv_patch_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
template<class Type> class inletOutletFvPatchField;
makePatchTypeFieldTypedefs(inletOutlet);
}  // namespace mousse
#endif
