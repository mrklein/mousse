// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_mag_sqr_grad_grad.cpp
#ifndef fvc_mag_sqr_grad_grad_hpp_
#define fvc_mag_sqr_grad_grad_hpp_
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
