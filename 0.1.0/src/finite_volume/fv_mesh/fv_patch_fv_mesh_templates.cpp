// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_patch.hpp"
// Member Functions 
template<class GeometricField, class Type>
const typename GeometricField::PatchFieldType& mousse::fvPatch::lookupPatchField
(
  const word& name,
  const GeometricField*,
  const Type*
) const
{
  return patchField<GeometricField, Type>
  (
    boundaryMesh().mesh().objectRegistry::template
      lookupObject<GeometricField>(name)
  );
}
