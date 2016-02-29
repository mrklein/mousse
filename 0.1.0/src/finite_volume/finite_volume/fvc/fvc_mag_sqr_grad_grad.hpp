#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_MAG_SQR_GRAD_GRAD_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_MAG_SQR_GRAD_GRAD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_mag_sqr_grad_grad.cpp
#include "vol_fields_fwd.hpp"
namespace mousse
{
namespace fvc
{
  template<class Type>
  tmp<volScalarField> magSqrGradGrad
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<volScalarField> magSqrGradGrad
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_mag_sqr_grad_grad.cpp"
#endif
#endif
