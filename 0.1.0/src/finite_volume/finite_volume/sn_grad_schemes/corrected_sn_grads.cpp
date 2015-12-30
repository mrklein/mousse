// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "corrected_sn_grad.hpp"
#include "fv_mesh.hpp"
makeSnGradScheme(correctedSnGrad)
template<>
mousse::tmp<mousse::surfaceScalarField>
mousse::fv::correctedSnGrad<mousse::scalar>::correction
(
  const volScalarField& vsf
) const
{
  return fullGradCorrection(vsf);
}
template<>
mousse::tmp<mousse::surfaceVectorField>
mousse::fv::correctedSnGrad<mousse::vector>::correction
(
  const volVectorField& vvf
) const
{
  return fullGradCorrection(vvf);
}
