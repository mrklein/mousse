// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_corrected_sn_grad.hpp"
#include "surface_fields.hpp"
#include "fv_mesh.hpp"


MAKE_SN_GRAD_SCHEME(faceCorrectedSnGrad)

template<>
mousse::tmp<mousse::surfaceScalarField>
mousse::fv::faceCorrectedSnGrad<mousse::scalar>::correction
(
  const volScalarField& vsf
) const
{
  return fullGradCorrection(vsf);
}


template<>
mousse::tmp<mousse::surfaceVectorField>
mousse::fv::faceCorrectedSnGrad<mousse::vector>::correction
(
  const volVectorField& vvf
) const
{
  return fullGradCorrection(vvf);
}
