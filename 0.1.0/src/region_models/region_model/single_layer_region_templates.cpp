// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "single_layer_region.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "mapped_fixed_internal_value_fv_patch_fields.hpp"
#include "mapped_fixed_pushed_internal_value_fv_patch_fields.hpp"
// Protected Member Functions 
template<class Type>
mousse::wordList
mousse::regionModels::singleLayerRegion::mappedFieldAndInternalPatchTypes() const
{
  wordList bTypes(regionMesh().boundaryMesh().size());
  bTypes = zeroGradientFvPatchField<Type>::typeName;
  FOR_ALL(intCoupledPatchIDs_, i)
  {
    const label patchI = intCoupledPatchIDs_[i];
    bTypes[patchI] = mappedFixedInternalValueFvPatchField<Type>::typeName;
  }
  return bTypes;
}
template<class Type>
mousse::wordList
mousse::regionModels::singleLayerRegion::mappedPushedFieldPatchTypes() const
{
  wordList bTypes(regionMesh().boundaryMesh().size());
  bTypes = zeroGradientFvPatchField<Type>::typeName;
  FOR_ALL(intCoupledPatchIDs_, i)
  {
    const label patchI = intCoupledPatchIDs_[i];
    bTypes[patchI] =
      mappedFixedPushedInternalValueFvPatchField<Type>::typeName;
  }
  return bTypes;
}
