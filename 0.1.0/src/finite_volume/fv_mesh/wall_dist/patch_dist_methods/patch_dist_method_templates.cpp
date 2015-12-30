// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_dist_method.hpp"
// Static Functions 
template<class Type>
mousse::wordList mousse::patchDistMethod::patchTypes
(
  const fvMesh& mesh,
  const labelHashSet& patchIDs
)
{
  wordList yTypes
  (
    mesh.boundary().size(),
    zeroGradientFvPatchField<Type>::typeName
  );
  forAllConstIter(labelHashSet, patchIDs, iter)
  {
    yTypes[iter.key()] = fixedValueFvPatchField<Type>::typeName;
  }
  return yTypes;
}
