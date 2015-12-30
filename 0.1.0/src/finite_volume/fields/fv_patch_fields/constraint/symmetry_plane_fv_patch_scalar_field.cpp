// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetry_plane_fv_patch_field.hpp"
#include "vol_fields.hpp"
// Member Functions 
template<>
mousse::tmp<mousse::scalarField>
mousse::symmetryPlaneFvPatchField<mousse::scalar>::snGrad() const
{
  return tmp<scalarField >(new scalarField(size(), 0.0));
}
template<>
void mousse::symmetryPlaneFvPatchField<mousse::scalar>::evaluate
(
  const Pstream::commsTypes
)
{
  if (!updated())
  {
    updateCoeffs();
  }
  scalarField::operator=(patchInternalField());
  transformFvPatchField<scalar>::evaluate();
}
