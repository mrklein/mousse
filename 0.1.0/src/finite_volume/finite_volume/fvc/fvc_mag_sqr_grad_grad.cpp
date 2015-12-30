// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_mag_sqr_grad_grad.hpp"
#include "fvc_grad.hpp"
#include "fv_mesh.hpp"
namespace mousse
{
namespace fvc
{
template<class Type>
tmp<volScalarField> magSqrGradGrad
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<volScalarField> tMagSqrGradGrad
  (
    magSqr(fvc::grad(fvc::grad(vf.component(0))))
  );
  // Loop over other vector field components
  for (direction cmpt = 1; cmpt < pTraits<Type>::nComponents; cmpt++)
  {
    tMagSqrGradGrad() += magSqr(fvc::grad(fvc::grad(vf.component(cmpt))))();
  }
  return tMagSqrGradGrad;
}
template<class Type>
tmp<volScalarField>
magSqrGradGrad
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
  tmp<volScalarField> tMagSqrGradGrad(fvc::magSqrGradGrad(tvf()));
  tvf.clear();
  return tMagSqrGradGrad;
}
}  // namespace fvc
}  // namespace mousse
