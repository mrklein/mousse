// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multivariate_gauss_convection_scheme.hpp"
#include "gauss_convection_scheme.hpp"
#include "fv_matrices.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
multivariateGaussConvectionScheme<Type>::interpolate
(
  const surfaceScalarField& faceFlux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  return gaussConvectionScheme<Type>
  (
    this->mesh(),
    faceFlux,
    tinterpScheme_()(vf)
  ).interpolate(faceFlux, vf);
}
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
multivariateGaussConvectionScheme<Type>::flux
(
  const surfaceScalarField& faceFlux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  return gaussConvectionScheme<Type>
  (
    this->mesh(),
    faceFlux,
    tinterpScheme_()(vf)
  ).flux(faceFlux, vf);
}
template<class Type>
tmp<fvMatrix<Type> >
multivariateGaussConvectionScheme<Type>::fvmDiv
(
  const surfaceScalarField& faceFlux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  return gaussConvectionScheme<Type>
  (
    this->mesh(),
    faceFlux,
    tinterpScheme_()(vf)
  ).fvmDiv(faceFlux, vf);
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
multivariateGaussConvectionScheme<Type>::fvcDiv
(
  const surfaceScalarField& faceFlux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  return gaussConvectionScheme<Type>
  (
    this->mesh(),
    faceFlux,
    tinterpScheme_()(vf)
  ).fvcDiv(faceFlux, vf);
}
}  // namespace fv
}  // namespace mousse
