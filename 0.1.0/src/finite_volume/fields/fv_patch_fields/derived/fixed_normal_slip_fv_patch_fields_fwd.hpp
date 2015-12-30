// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef fixed_normal_slip_fv_patch_fields_fwd_hpp_
#define fixed_normal_slip_fv_patch_fields_fwd_hpp_
#include "field_types.hpp"
namespace mousse
{
template<class Type> class fixedNormalSlipFvPatchField;
typedef fixedNormalSlipFvPatchField<vector> fixedNormalSlipFvPatchVectorField;
typedef fixedNormalSlipFvPatchField<tensor> fixedNormalSlipFvPatchTensorField;
}  // namespace mousse
#endif
