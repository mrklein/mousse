// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "transform_fv_patch_field.hpp"


// Member Functions 
template<>
mousse::tmp<mousse::scalarField>
mousse::transformFvPatchField<mousse::scalar>::valueInternalCoeffs
(
  const tmp<scalarField>&
) const
{
  return tmp<scalarField>{new scalarField{size(), 1.0}};
}


template<>
mousse::tmp<mousse::scalarField>
mousse::transformFvPatchField<mousse::scalar>::gradientInternalCoeffs() const
{
  return tmp<scalarField>{new scalarField{size(), 0.0}};
}

